/***************************************************************************//**
 * @file
 * @brief main.c
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"

#include "radio.h"
#include "token.h"

#include "retargetserial.h"

#include "main.h"
#include "ci.h"

#include "native_gecko.h"

#include "thunderboard/util.h"
#include "thunderboard/board.h"
#include "thunderboard/bap.h"
#include "thunderboard/si7021.h"
#include "thunderboard/hall.h"
#include "thunderboard/si1133.h"
#include "thunderboard/ccs811.h"
#include "thunderboard/mic.h"

#include "app.h"
#include "radio.h"
#include "radio_ble.h"

#define MIC_SAMPLE_RATE            8000
#define MIC_SAMPLE_BUFFER_SIZE     2048

static uint16_t micSampleBuffer[MIC_SAMPLE_BUFFER_SIZE];

static void     init                (bool radio);
static void     readTokens          (void);

uint16_t RADIO_xoTune = 344;

int main(void)
{
  /**************************************************************************/
  /* Device errata init                                                     */
  /**************************************************************************/
  CHIP_Init();

  /**************************************************************************/
  /* Read tokens and store settings                                         */
  /**************************************************************************/
  readTokens();

  /**************************************************************************/
  /* Board init                                                             */
  /**************************************************************************/
  init(true);

  printf("\r\n\r\n#### Thunderboard Sense BLE application %s-%d %07x ####\r\n",
         CI_VERSION,
         CI_BUILD,
         CI_COMMIT
         );

  /**************************************************************************/
  /* Application loop                                                       */
  /**************************************************************************/
  while (1) {
    struct gecko_cmd_packet* evt;
    /* Check for stack event. */
    evt = gecko_wait_event();
    /* Run application and event handler. */
    appHandleEvents(evt);
  }
}

void MAIN_initSensors()
{
  uint8_t bapDeviceId;
  uint32_t status;
  HALL_Config hallConf;

  SI7021_init();
  SI1133_init();
  BAP_init(&bapDeviceId);
  printf("Pressure sensor: %s detected\r\n",
         bapDeviceId == BAP_DEVICE_ID_BMP280 ? "BMP280" : "BMP180");

  status = HALL_init();
  printf("HALL init status: %x\r\n", (unsigned int)status);

  if ( status == HALL_OK ) {
    hallConf.threshold    = 3.0f;
    hallConf.hysteresis   = 0.5f;  // Use default
    hallConf.polarity     = HALL_CONFIG_POLARITY_OMNIPOLAR;
    hallConf.outputInvert = false;
    HALL_configure(&hallConf);
  }

  HALL_registerCallback(RADIO_bleHallStateCallback);

  if ( UTIL_isLowPower() == false ) {
    CCS811_init();
    status = CCS811_startApplication();
    if ( status == CCS811_OK ) {
      status = CCS811_setMeasureMode(CCS811_MEASURE_MODE_DRIVE_MODE_10SEC);
    }
    printf("CCS811 init status: %x\r\n", (unsigned int)status);
  }

  MIC_init(MIC_SAMPLE_RATE, micSampleBuffer, MIC_SAMPLE_BUFFER_SIZE);

  BOARD_rgbledSetRawColor(0, 0, 0);

  return;
}

void MAIN_deInitSensors()
{
  SI7021_deInit();
  HALL_deInit();
  SI1133_deInit();
  BAP_deInit();
  BOARD_envSensEnable(false);

  if ( UTIL_isLowPower() == false ) {
    CCS811_deInit();
  }

  MIC_deInit();

  BOARD_ledSet(0);
  BOARD_rgbledSetRawColor(0, 0, 0);
  BOARD_rgbledEnable(false, 0xFF);

  return;
}

#define RADIO_XO_TUNE_VALUE 344
void init(bool radio)
{
  uint8_t  supplyType;
  float    supplyVoltage;
  float    supplyIR;

  /**************************************************************************/
  /* Module init                                                            */
  /**************************************************************************/
  UTIL_init();
  BOARD_init();

  UTIL_supplyProbe();
  UTIL_supplyGetCharacteristics(&supplyType, &supplyVoltage, &supplyIR);

  printf("\r\n");
  printf("Supply voltage : %.3f\r\n", supplyVoltage);
  printf("Supply IR      : %.3f\r\n", supplyIR);
  printf("Supply type    : ");
  if ( supplyType == UTIL_SUPPLY_TYPE_USB ) {
    printf("USB\r\n");
  } else if ( supplyType == UTIL_SUPPLY_TYPE_AA ) {
    printf("Dual AA batteries\r\n");
  } else if ( supplyType == UTIL_SUPPLY_TYPE_AAA ) {
    printf("Dual AAA batteries\r\n");
  } else if ( supplyType == UTIL_SUPPLY_TYPE_CR2032 ) {
    printf("CR2032\r\n");
  } else if ( supplyType == UTIL_SUPPLY_TYPE_WSTK ) {
    printf("WSTK (VAEM)\r\n");
  } else {
    printf("Unknown\r\n");
  }

  /**************************************************************************/
  /* System clock and timer init                                            */
  /**************************************************************************/
  if ( radio ) {
    RADIO_bleStackInit();
  } else {
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  }

  /* Re-initialize serial port and UTIL which depend on the HF clock frequency */
  RETARGET_SerialInit();
  UTIL_init();
  BOARD_init();

  /* In low power mode, sensors are enabled and disabled when entering/leaving connected mode */
  if ( !UTIL_isLowPower() ) {
    MAIN_initSensors();
  }

  GPIO_PinModeSet(gpioPortD, 14, gpioModeInput, 0);
  GPIO_PinModeSet(gpioPortD, 15, gpioModeInput, 0);

  return;
}

void readTokens(void)
{
  uint16_t t16;

  /* Dump tokens */
  t16 = TOKEN_getU16(SB_RADIO_CTUNE);
  if ( t16 != 0xFFFF ) {
    RADIO_xoTune = t16;
    printf("\r\nSB_RADIO_CTUNE = %d\r\n", t16);
  }
  t16 = TOKEN_getU16(SB_RADIO_CHANNEL);
  if ( t16 != 0xFFFF ) {
    printf("SB_RADIO_CHANNEL = %d\r\n", t16);
  }
  t16 = TOKEN_getU16(SB_RADIO_OUTPUT_POWER);
  if ( t16 != 0xFFFF ) {
    printf("SB_RADIO_OUTPUT_POWER = %d\r\n", t16);
  }
  printf("\r\n");

  return;
}
