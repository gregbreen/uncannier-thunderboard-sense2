#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "appl_ver.h"

/* BG stack headers */
#include "bg_types.h"
#include "infrastructure.h"

#include "native_gecko.h"

/* BG stack headers */
#include "gatt_db.h"

/* application specific headers */
#include "connection.h"

#include "batt.h"

/* BLE services */
#include "iaq.h"
#include "es.h"
#include "ui.h"
#include "aio.h"
#include "imus.h"
#include "power.h"
#include "hallservice.h"

#include "app_ble_adv.h"
#include "app_callback.h"

/* Own header */
#include "app_ble.h"

static void bleDeviceNameRead(void);

AppCfgGattServerAttributeValue_t appCfgGattServerAttributeValue[] =
{
  { 0, NULL }
};

AppCfgGattServerUserReadRequest_t appCfgGattServerUserReadRequest[] =
{
  { gattdb_device_name, bleDeviceNameRead },
  { gattdb_batt_measurement, battRead },
  { gattdb_es_uvindex, esReadUVIndex },
  { gattdb_es_pressure, esReadPressure },
  { gattdb_es_temperature, esReadTemperature },
  { gattdb_es_humidity, esReadHumidity },
  { gattdb_es_ambient_light, esReadAmbientLight },
  { gattdb_es_sound_level, esReadSoundLevel },
  /*{ gattdb_es_control_point,  esReadControlPoint },*/
  { gattdb_iaq_eco2, iaqReadECO2 },
  { gattdb_iaq_tvoc, iaqReadTVOC },
  /*{ gattdb_iaq_tvoc,          uiReadButtons },*/
  { gattdb_ui_buttons, uiReadButtons },
  { gattdb_ui_rgbleds, uiRGBLEDRead },
  /*{ gattdb_iaq_control_point, iaqReadControlPoint },*/
  { gattdb_aio_digital_in, aioDigitalInRead },
  { gattdb_aio_digital_out, aioDigitalOutRead },
  { gattdb_power_source_type, powerSourceTypeRead },
  { gattdb_hall_field_strength, hallServiceFieldRead },
  { gattdb_hall_state, hallServiceStateRead },
  { 0, NULL }
};

AppCfgGattServerUserWriteRequest_t appCfgGattServerUserWriteRequest[] =
{
  { gattdb_es_control_point, esControlPointWrite },
  { gattdb_iaq_control_point, iaqControlPointWrite },
  { gattdb_ui_leds, uiLEDWrite },
  { gattdb_ui_rgbleds, uiRGBLEDWrite },
  { gattdb_aio_digital_out, aioDigitalOutWrite },
  { gattdb_imu_control_point, imuControlPointWrite },
  { gattdb_hall_control_point, hallServiceControlPointWrite },
  { 0, NULL }
};

AppCfgGattServerCharStatus_t appCfgGattServerCharStatus[] =
{
  { gattdb_batt_measurement, battCharStatusChange },
  { gattdb_es_control_point, esControlPointChange },
  { gattdb_iaq_control_point, iaqControlPointChange },
  { gattdb_aio_digital_in, aioDigitalInCharStatusChange },
  { gattdb_imu_acceleration, imuAccelerationStatusChange },
  { gattdb_imu_orientation, imuOrientationStatusChange },
  { gattdb_imu_control_point, imuControlPointStatusChange },
  { gattdb_hall_state, hallServiceStateStatusChange },
  { gattdb_hall_field_strength, hallServiceFieldStatusChange },
  { gattdb_hall_control_point, hallServiceControlPointChange },
  /*{ gattdb_ui_buttons,        uiButtonChange },*/
  { 0, NULL }
};

AppCfgGattServerConfirmation_t appCfgGattServerConfirmation[] =
{
  { 0, NULL }
};

size_t appCfgGattServerAttributeValueSize   = COUNTOF(appCfgGattServerAttributeValue) - 1;
size_t appCfgGattServerUserReadRequestSize  = COUNTOF(appCfgGattServerUserReadRequest) - 1;
size_t appCfgGattServerUserWriteRequestSize = COUNTOF(appCfgGattServerUserWriteRequest) - 1;
size_t appCfgGattServerCharStatusSize       = COUNTOF(appCfgGattServerCharStatus) - 1;
size_t appCfgGattServerConfirmationSize     = COUNTOF(appCfgGattServerConfirmation) - 1;

#define APP_BLE_SYSTEM_ID_SIZE 8
static uint8_t  systemId[APP_BLE_SYSTEM_ID_SIZE];
static char     devName[APP_BLE_ADV_DEVICE_NAME_MAX_LENGTH + 1];
static uint32_t uniqueId;

static uint8_t  boardRevision[8];
static uint8_t  boardSerial[16];
static uint8_t  firmwareRevision[16];

void appBleInit(void)
{
  uint8_t btAddr[6];
  struct gecko_msg_system_get_bt_address_rsp_t *btAddrRsp;

  // Extract unique ID from BT Address
  btAddrRsp = gecko_cmd_system_get_bt_address();
  memcpy(btAddr, btAddrRsp->address.addr, 6);
  uniqueId = 0xFFFFFF & *((uint32*) btAddr);

  // Pad and reverse unique ID to get System ID
  systemId[0] = btAddr[5];
  systemId[1] = btAddr[4];
  systemId[2] = btAddr[3];
  systemId[3] = 0xFF;
  systemId[4] = 0xFE;
  systemId[5] = btAddr[2];
  systemId[6] = btAddr[1];
  systemId[7] = btAddr[0];

  printf("BLE address       : %02X:%02X:%02X:%02X:%02X:%02X\r\n", btAddr[5], btAddr[4], btAddr[3], btAddr[2], btAddr[1], btAddr[0]);
  gecko_cmd_gatt_server_write_attribute_value(gattdb_system_id,
                                              0,
                                              APP_BLE_SYSTEM_ID_SIZE,
                                              systemId);

  sprintf((char *)firmwareRevision, "%d.%d.%d", APP_VERSION_MAJOR, APP_VERSION_MINOR, APP_VERSION_PATCH);
  printf("Firmware revision : %s\r\n", firmwareRevision);
  gecko_cmd_gatt_server_write_attribute_value(gattdb_firmware_rev,
                                              0,
                                              strlen((char const *)firmwareRevision),
                                              firmwareRevision);

  sprintf((char *)boardRevision, "A00");
  printf("Board revision    : %s\r\n", boardRevision);
  gecko_cmd_gatt_server_write_attribute_value(gattdb_board_rev,
                                              0,
                                              strlen((char const *)boardRevision),
                                              boardRevision);

  sprintf((char *)boardSerial, "%d", 1234);
  printf("Board serial      : %s\r\n", boardSerial);
  gecko_cmd_gatt_server_write_attribute_value(gattdb_board_serial,
                                              0,
                                              strlen((char const *)boardSerial),
                                              boardSerial);

  appBleAdvInit();
  snprintf(devName, sizeof(devName), APP_BLE_ADV_DEVICE_NAME_FORMAT_STRING, (int)(uniqueId & 0xFFFF));
  printf("Device name       : %s [%d]\r\n", devName, strlen(devName));
  appBleAdvSetDevName(devName);

  appBleAdvInit();
  appBleAdvSetId(uniqueId);

  appBleAdvStart();

  return;
}

#if 0
// Check if Persistent Storage contains name already, if not, store it there
psResp = gecko_cmd_flash_ps_load(DEVNAME_PS_KEY);
if (psResp->result) {
  // Key not found
  // Concatenate name with unique ID and zero terminate
  snprintf(devName, DEVNAME_DEFAULT_LEN + 1, DEVNAME_DEFAULT_STRING, (uniqueId & 0xFFFF));

  gecko_cmd_flash_ps_save(DEVNAME_PS_KEY, DEVNAME_MAX_LEN + 1, (uint8_t *)devName);
} else {
  memcpy(devName, psResp->value.data, DEVNAME_MAX_LEN + 1);
}
#endif

void appCfgConnectionClosedEvent(uint8_t connection, uint16_t reason)
{
  conConnectionClosed();
  advInit();
  appBleAdvStart();
//   emberAfMainPluginsInit();
  battInit();

  return;
}

void appCfgResetEvent(void)
{
  advInit();
  appBleAdvStart();
  conConnectionInit();
//   emberAfMainPluginsInit();
  battInit();

  resetEventCallback();

  return;
}

void appCfgConnectionOpenedEvent(uint8_t connection, uint8_t bonding)
{
  conConnectionStarted(connection, bonding);

  return;
}

void appCfgGattProcCompEvent(uint16_t result)
{
  return;
}

void appCfgGattBondedEvent(struct gecko_msg_sm_bonded_evt_t* bond)
{
  return;
}

void appCfgServiceFoundEvent(struct gecko_msg_gatt_service_evt_t* srv)
{
  return;
}

void appCfgChrFoundEvent(struct gecko_msg_gatt_characteristic_evt_t* chr)
{
  return;
}

void appCfgChrValueReceivedEvent(struct gecko_msg_gatt_characteristic_value_evt_t* chr)
{
  return;
}

void appCfgScanResponse(struct gecko_cmd_packet *evt)
{
#ifdef SILABS_AF_PLUGIN_CENTRAL
  centralScanResponse(evt->data.evt_le_gap_scan_response.data.data,
                      evt->data.evt_le_gap_scan_response.data.len,
                      evt->data.evt_le_gap_scan_response.address,
                      evt->data.evt_le_gap_scan_response.address_type);
#endif /* SILABS_AF_PLUGIN_CABLE_CLIENT */
}

extern uint8_t bleDeviceName[];

static void bleDeviceNameRead(void)
{
  uint8_t valueLength;
  const uint8_t *valueData;

  valueData   = (const uint8_t *)bleDeviceName;
  valueLength = strlen((char const*)valueData);

  printf("bleDeviceNameRead(): %s\r\n", valueData);

  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_device_name,
    0,
    valueLength,
    valueData
    );

  return;
}

/*

 #define DEVNAME_NAME_TOO_LONG_ERROR  0x0A
 #define DEVNAME_NAME_NOT_FOUND_ERROR 0x0B

 #define SYSTEM_ID_BYTES 8

 #define DEVNAME_PS_KEY 0x4000

   static void bleDeviceNameWrite( uint8array *writeValue )
   {

   char devName[DEVNAME_MAX_LEN+1];

   if( writeValue->len <= DEVNAME_MAX_LEN )
   {

      gecko_cmd_gatt_server_send_user_write_response( conGetConnectionId(),
                                                      gattdb_device_name,
                                                      0);

      memcpy( devName, writeValue->data, writeValue->len );
      devName[ writeValue->len ] = 0;

      gecko_cmd_flash_ps_save(DEVNAME_PS_KEY, writeValue->len+1, (uint8_t *)devName);

      appBleAdvSetDevName( devName );

      appBleAdvInit();

   }
   else
   {
      gecko_cmd_gatt_server_send_user_write_response(conGetConnectionId(),
                                                     gattdb_device_name,
                                                     DEVNAME_NAME_TOO_LONG_ERROR);
   }

   return;

   }
 */
