#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "em_core.h"
#include "em_system.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_ldma.h"
#include "em_prs.h"
#include "em_rtcc.h"

#include "thunderboard/util.h"
#include "thunderboard/ccs811.h"
#include "thunderboard/si7021.h"
#include "thunderboard/hall.h"
#include "thunderboard/si1133.h"
#include "thunderboard/bap.h"
#include "thunderboard/board.h"
#include "thunderboard/imu/imu.h"
#include "thunderboard/mic.h"

#include "main.h"

#include "aio.h"
#include "imus.h"
#include "hallservice.h"

/* BG stack headers */
/*#include "bg_types.h"*/
#include "native_gecko.h"
#include "gatt_db.h"

/* application specific files */
#include "app.h"

#include "app_ble_adv.h"
#include "radio.h"
#include "radio_ble.h"

#include "pti.h"

/* Each iteration is 100 ms  */
#define APP_ADV_SLEEP_TIMEOUT_ITERATIONS           300

#define APP_STATE_UNKNOWN     0
#define APP_STATE_ADVERTISING 1
#define APP_STATE_CONNECTED   2

#define MAX_CONNECTIONS 4
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

#if defined SILABS_AF_PLUGIN_IDLE_SLEEP
/* Gecko configuration parameters (see gecko_configuration.h) */
static gecko_configuration_t config =
{
  /* USARTs can be configured (see datasheet for signal location options */

  /* I2C can be configured (see datasheet for signal location options. flags field must be set to
     I2C_FLAGS_ENABLED to enable peripheral */

  /* GPIO default configuration can be set (see EFR Reference Manual)
     CTRL - Port Control Register
     MODEL - Port Pin Mode Low Register
     MODEH - Port Pin Mode High Register
     DOUT - Port Data Out Register */

  /* Power management (DC/DC regulator) can be configured, for initial setting see EFR Reference
   * Manual and em_emu.h file */

  /* High Frequency Oscillator and Low Frequency Oscillators  can be configured, for initial setting
   *  see Clock Management in reference manual and em_cmu.h file */

#ifdef SILABS_AF_PLUGIN_IDLE_SLEEP
  .sleep =
  {
    0, 0, 0
  }
#endif /* SILABS_AF_PLUGIN_IDLE_SLEEP */
};
#else /* !defined SILABS_AF_PLUGIN_PTI && !defined SILABS_AF_PLUGIN_IDLE_SLEEP */
static gecko_configuration_t config;
#endif /* defined SILABS_AF_PLUGIN_PTI || defined SILABS_AF_PLUGIN_IDLE_SLEEP */

uint16_t RADIO_eco2 = 0;
uint16_t RADIO_tvoc = 0;
uint32_t RADIO_rhData = 50000;
int32_t  RADIO_tempData = 25000;
uint8_t  RADIO_uvIndex = 0;
uint32_t RADIO_ambLight = 0;
float    RADIO_pressure = 0;

float    RADIO_soundLevel            = -60;
uint32_t RADIO_soundLevelBufferCount = 0;
float    RADIO_soundLevelAccumulated = 0;

float    RADIO_magneticField = 0;
volatile bool     RADIO_hallStateChanged = true;
volatile uint8_t  RADIO_hallState = HALLS_STATE_OPEN;

extern uint16_t RADIO_xoTune;

EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
gecko_sleep_config_t sleepConfig;

static void advertisingIteration(uint32_t loopCount);
static void connectionActiveIteration(uint32_t loopCount);
static void mainEfr32StackInitCallback(void);
const void *RADIO_bleGetSleepConfig(void);

static int blinkCount;

void advBlink(void);

void RADIO_bleStackInit(void)
{
  blinkCount = 0;

  RADIO_bleChipInit();

#if (HAL_PTI_ENABLE == 1) || defined(FEATURE_PTI_SUPPORT)
  configEnablePti();
#endif

  /* Perform initialisation of the stack. */
  mainEfr32StackInitCallback();

  return;
}

void RADIO_bleStackLoopIteration(void)
{
  static uint32_t loopCount = 0;
  static uint8_t appState = APP_STATE_UNKNOWN;
  bool imuIntIsActive = false;

  ++loopCount;

  if ( IMU_isDataReadyFlag() ) {
    IMU_clearDataReadyFlag();
    imuIntIsActive = true;
  }

  if ( appBleAdvIsActive() ) {
    if (appState != APP_STATE_ADVERTISING ) {
      if ( UTIL_isLowPower() ) {
        MAIN_deInitSensors();
      }
      appState = APP_STATE_ADVERTISING;
    }

    advertisingIteration(loopCount);
  } else {
    if ( appState != APP_STATE_CONNECTED ) {
      if ( UTIL_isLowPower() ) {
        MAIN_initSensors();
      }
      appState = APP_STATE_CONNECTED;
      BOARD_ledSet(0);
    }
    connectionActiveIteration(loopCount);
  }

  RADIO_readImuData(imuIntIsActive);
}

void advertisingIteration(uint32_t loopCount)
{
#ifndef DEBUG
  uint32_t sleepTimeout;
  static uint32_t previousAdvLC = 0;
  static uint32_t advStartedLC = 0;

  if ( previousAdvLC + 1 != loopCount ) {
    advStartedLC = loopCount;
  }
  previousAdvLC = loopCount;

  sleepTimeout = APP_ADV_SLEEP_TIMEOUT_ITERATIONS;
#endif

  advBlink();

#ifndef DEBUG
  if ( (loopCount - advStartedLC) > sleepTimeout ) {
    /* printf("\r\nSleep! %u %u", loopCount, advStartedLC); */
    if ( !UTIL_isLowPower() ) {
      MAIN_deInitSensors();
    }
    BOARD_imuClearIRQ();
    BOARD_gasSensorClearIRQ();
    BOARD_alsClearIRQ();
    BOARD_ledSet(0x00);
    UTIL_shutdown();
  }
#endif

  return;
}

#define RADIO_BLE_ADV_ON_COUNT  (1)
#define RADIO_BLE_ADV_OFF_COUNT (10)
void advBlink(void)
{
  int onCount = RADIO_BLE_ADV_ON_COUNT;
  int offCount = RADIO_BLE_ADV_OFF_COUNT;

  if ( blinkCount == onCount ) {
    BOARD_ledSet(0x00);
  } else if ( blinkCount >= offCount || blinkCount == 0 ) {
    BOARD_ledSet(0x02);
    blinkCount = 0;
  }
  blinkCount++;

  return;
}

void connectionActiveIteration(uint32_t loopCount)
{
  static bool expectAlsData = false;
  static uint8_t buttonState = 0;
  static uint8_t lastHallState = HALLS_STATE_OPEN;
  uint8_t irqStatus = 0;
  uint8_t state;
  uint8_t hallState;
  bool    hallStateChanged;
  float   lux = 0;
  float   uv = 0;
  float   micVar;

  blinkCount = 0;

  /* Check if the button state has changed */
  state = BOARD_pushButtonGetState();
  if ( state != buttonState ) {
    buttonState = state;
    aioDigitalInUpdate();
  }

  /* RH / T sensor read */
  if ( (loopCount % 30) == 0 ) {
    /* Measure the environmental temp and RH */
    /* If there is an error during the measurement use 25C and 50% RH */
    if ( SI7021_measure(&RADIO_rhData, &RADIO_tempData) != SI7021_OK ) {
      RADIO_tempData = 25000;
      RADIO_rhData = 50000;
    }

    if ( BAP_getPressure(&RADIO_pressure) != BAP_OK) {
      RADIO_pressure = 0;
    }

    /* Perform CCS811 measurement if not gated by supply */
    if ( UTIL_isLowPower() == false ) {
      if ( CCS811_isDataAvailable() ) {
        CCS811_getMeasurement(&RADIO_eco2, &RADIO_tvoc);
        CCS811_setEnvData(RADIO_tempData, RADIO_rhData);
      }
    }

    /* Start an ALS measurement */
    SI1133_measurementForce();
    expectAlsData = true;

    printf("\r\n.");
  }

  if ( expectAlsData ) {
    /* Check if the conversion finished on all channels */
    SI1133_getIrqStatus(&irqStatus);
    if (irqStatus == 0x0F) {
      expectAlsData = false;
      SI1133_getMeasurement(&lux, &uv);
      RADIO_ambLight = (uint32_t)(lux * 100);
      RADIO_uvIndex = (uint8_t)uv;
    }
  }

  if ( !MIC_isBusy() ) {
    MIC_getSoundLevel(&micVar);
    MIC_start(2048);

    RADIO_soundLevelAccumulated += micVar;
    RADIO_soundLevelBufferCount++;

    RADIO_soundLevel = RADIO_soundLevelAccumulated / RADIO_soundLevelBufferCount;
    RADIO_soundLevel = 10.0f * log10f(RADIO_soundLevel);
    RADIO_soundLevel += 120.0f;
  }

  /* Read magnetic field strength and set door state */
  if ( (loopCount % 1) == 0 || RADIO_hallStateChanged ) {
    HALL_measure(20000, &RADIO_magneticField);

    hallServiceFieldUpdated(RADIO_magneticField);

    /* Disable interrupts while reading/updating state variables */
    CORE_CRITICAL_IRQ_DISABLE();
    hallState = RADIO_hallState;
    hallStateChanged = RADIO_hallStateChanged;
    if ( RADIO_hallStateChanged ) {
      RADIO_hallStateChanged = false;
    }
    CORE_CRITICAL_IRQ_ENABLE();

    /* We need to use the field strength to discern between open and tamper states */
    if ( hallState == HALLS_STATE_OPEN ) {
      if ( abs((int)RADIO_magneticField) >= (int)HALL_getTamperLevel() ) {
        hallState = HALLS_STATE_TAMPER;
      } else {
        hallState = HALLS_STATE_OPEN;
      }
    }

    if ( lastHallState != hallState ) {
      hallStateChanged = true;
      lastHallState = hallState;
    }

    if ( hallStateChanged ) {
      hallServiceStateChanged(hallState);
    }
  }

  return;
}

void RADIO_readImuData(bool imuIntIsActive)
{
  if ( IMU_isDataReadyFlag() ) {
    IMU_clearDataReadyFlag();
    imuIntIsActive = true;
  }

  if ( imuIsAccelerationNotification() || imuIsOrientationNotification() ) {
    if ( imuIntIsActive ) {
      if ( IMU_isDataReady() ) {
        IMU_update();
      }
    }
  }
}

void RADIO_bleHallStateCallback(bool pinHigh)
{
  RADIO_hallStateChanged = true;

  if ( pinHigh ) {
    RADIO_hallState = HALLS_STATE_CLOSED;
  } else {
    RADIO_hallState = HALLS_STATE_OPEN;
  }
}

void RADIO_bleChipInit(void)
{
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
  CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;

  if ( UTIL_isLowPower() == true ) {
    printf("BLE stack sleep enabled!\r\n");
    sleepConfig.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE;
  } else {
    sleepConfig.flags = 0;
  }

  /*************************************************************************/
  /* DC/DC */
  /*************************************************************************/
  dcdcInit.powerConfig = emuPowerConfig_DcdcToDvdd;
  dcdcInit.dcdcMode = emuDcdcMode_LowNoise;
  dcdcInit.mVout = 1800;
  dcdcInit.em01LoadCurrent_mA = 5;
  dcdcInit.em234LoadCurrent_uA = 10;
  dcdcInit.maxCurrent_mA = 160;
  dcdcInit.anaPeripheralPower = emuDcdcAnaPeripheralPower_AVDD;
  dcdcInit.reverseCurrentControl = emuDcdcLnFastTransient;

  EMU_DCDCInit(&dcdcInit);

  /*************************************************************************/
  /* HFXO */
  /*************************************************************************/
  hfxoInit.autoStartEm01 = 1;
  hfxoInit.ctuneSteadyState = RADIO_xoTune;
  CMU_HFXOInit(&hfxoInit);

  SystemHFXOClockSet(38400000);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
  CMU_ClockEnable(cmuClock_CORELE, true);

  /*************************************************************************/
  /* LFXO */
  /*************************************************************************/
  CMU_LFXOInit(&lfxoInit);

  CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
  CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);

  CMU_ClockEnable(cmuClock_GPCRC, true);
  CMU_ClockEnable(cmuClock_LDMA, true);
  CMU_ClockEnable(cmuClock_PRS, true);
  CMU_ClockEnable(cmuClock_RTCC, true);

  return;
}

const void *RADIO_bleChipGetDCDCConfig(void)
{
  return (const void *)&dcdcInit;
}

const void *RADIO_bleGetSleepConfig(void)
{
  return (const void *)&sleepConfig;
}

static void mainEfr32StackInitCallback(void)
{
  gecko_sleep_config_t *gsc;

  /* Put the SPI flash into Deep Power Down mode for those radio boards where it is available */
  BOARD_flashDeepPowerDown();

  gsc          = (gecko_sleep_config_t *) RADIO_bleGetSleepConfig();
  config.sleep.flags = gsc->flags;

  config.gattdb = &bg_gattdb_data;
  config.bluetooth.max_connections = MAX_CONNECTIONS;
  config.bluetooth.heap = bluetooth_stack_heap;
  config.bluetooth.heap_size = sizeof(bluetooth_stack_heap);
  config.ota.flags = 0;
  config.ota.device_name_len = 3;
  config.ota.device_name_ptr = "OTA";

  gecko_init(&config);

  return;
}
