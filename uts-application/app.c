/* standard library headers */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* em library */
#include "em_system.h"

/* BG stack headers */
#include "bg_types.h"
#include "gatt_db.h"
#include "native_gecko.h"

/* application specific headers */
#include "app_ble.h"
#include "app_timer.h"
#include "app_ble_adv.h"
#include "radio_ble.h"

/* Services */
#include "imus.h"
#include "batt.h"

/* Own header */
#include "app.h"

static uint8_t connection;

#define BLE_DEVICE_NAME_SIZE 32
uint16_t bleDeviceId;
uint8_t  bleDeviceName[BLE_DEVICE_NAME_SIZE];

void appServiceStart(void);
void appServiceStop(void);

void appInit(void)
{
  struct gecko_msg_system_get_bt_address_rsp_t* btAddr;

  /* Create the device name based on the 16-bit device ID */
  btAddr      = gecko_cmd_system_get_bt_address();
  bleDeviceId = *(uint16_t *) (btAddr->address.addr);

  /* Write 16-bit device ID to GATT DB. Default behaviour. */
  snprintf((char *)bleDeviceName, BLE_DEVICE_NAME_SIZE, APP_BLE_ADV_DEVICE_NAME_FORMAT_STRING, bleDeviceId);
  gecko_cmd_gatt_server_write_attribute_value(
    gattdb_device_name,
    0,
    strlen((const char *)bleDeviceName),
    &bleDeviceName[0]
    );

  appBleInit();

  gecko_cmd_hardware_set_soft_timer
  (
    TIMER_MS_2_TIMERTICK(100),
    SENSOR_READOUT_TIMER, false
  );

  return;
}

void appServiceStart(void)
{
  imuServiceStart();
  return;
}

void appServiceStop(void)
{
  imuServiceStop();
  return;
}

void appHandleEvents(struct gecko_cmd_packet *evt)
{
  uint8_t i; /* Cycle index variable */

  if (NULL == evt) {
    return;
  }

  switch (BGLIB_MSG_ID(evt->header)) {
    /* This event indicates the device has started and is ready to receive any command except
     *  Bluetooth-related commands */
    case gecko_evt_system_boot_id:
      appInit(); /* Has to be called first to set up graphics. */
      appCfgResetEvent();
      appBleAdvStart();
      break;

    /* This event indicates that a connection was closed. */
    case gecko_evt_le_connection_closed_id:
      appCfgConnectionClosedEvent(evt->data.evt_le_connection_closed.connection,
                                  evt->data.evt_le_connection_closed.reason);
      break;

    /* This event is triggered after the connection has been opened */
    /* todo Read Bonding status */
    case gecko_evt_le_connection_opened_id:
      connection = evt->data.evt_le_connection_opened.connection;
      appCfgConnectionOpenedEvent(connection, evt->data.evt_le_connection_opened.bonding);
      break;

    /* Procedure has been successfully completed or failed with error. */
    case gecko_evt_gatt_procedure_completed_id:
      appCfgGattProcCompEvent(evt->data.evt_gatt_procedure_completed.result);
      break;

    /* Value of attribute changed from the local database by remote GATT client */
    case gecko_evt_gatt_server_attribute_value_id:
      for (i = 0; i < appCfgGattServerAttributeValueSize; i++) {
        if ( (appCfgGattServerAttributeValue[i].charId
              == evt->data.evt_gatt_server_attribute_value.attribute)
             && (appCfgGattServerAttributeValue[i].fctn) ) {
          appCfgGattServerAttributeValue[i].fctn(
            &(evt->data.evt_gatt_server_attribute_value.value));
        }
      }
      break;

    /* Indicates the changed value of CCC or received characteristic confirmation */
    case gecko_evt_gatt_server_characteristic_status_id:
      /* Char status changed */
      if (evt->data.evt_gatt_server_characteristic_status.status_flags == 0x01) {
        for (i = 0; i < appCfgGattServerCharStatusSize; i++) {
          if ( (appCfgGattServerCharStatus[i].charId
                == evt->data.evt_gatt_server_characteristic_status.characteristic)
               && (appCfgGattServerCharStatus[i].fctn) ) {
            appCfgGattServerCharStatus[i].fctn(
              evt->data.evt_gatt_server_characteristic_status.connection,
              evt->data.evt_gatt_server_characteristic_status.client_config_flags);
          }
        }
      }
      /* Confirmation received */
      else if ((evt->data.evt_gatt_server_characteristic_status.status_flags == 0x02)
               /* must be a response to an indication*/
               && (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 2)) {
        for (i = 0; i < appCfgGattServerConfirmationSize; i++) {
          if ( (appCfgGattServerConfirmation[i].charId
                == evt->data.evt_gatt_server_characteristic_status.characteristic)
               && (appCfgGattServerConfirmation[i].fctn) ) {
            appCfgGattServerConfirmation[i].fctn(
              evt->data.evt_gatt_server_characteristic_status.connection);
          }
        }
      }
      break;

    /* This event indicates that a GATT service at the remote GATT database was discovered */
    case gecko_evt_gatt_service_id:
      appCfgServiceFoundEvent(&(evt->data.evt_gatt_service));
      break;

    /* This event indicates that a GATT characteristic was discovered from a remote GATT
     * database */
    case gecko_evt_gatt_characteristic_id:
      appCfgChrFoundEvent(&(evt->data.evt_gatt_characteristic));
      break;

    /* This event indicates that the value of a characteristic at the remote GATT server
     * was received */
    case gecko_evt_gatt_characteristic_value_id:
      appCfgChrValueReceivedEvent(&(evt->data.evt_gatt_characteristic_value));
      break;

    /* This event indicates that a remote GATT client is attempting to read a value of an
     *  attribute from the local GATT database, where the attribute was defined in the GATT
     *  XML firmware configuration file to have type="user". */
    case gecko_evt_gatt_server_user_read_request_id:
      for (i = 0; i < appCfgGattServerUserReadRequestSize; i++) {
        if ( (appCfgGattServerUserReadRequest[i].charId
              == evt->data.evt_gatt_server_user_read_request.characteristic)
             && (appCfgGattServerUserReadRequest[i].fctn) ) {
          appCfgGattServerUserReadRequest[i].fctn();
        }
      }
      break;

    /* This event indicates that a remote GATT client is attempting to write a value of an
     * attribute in to the local GATT database, where the attribute was defined in the GATT
     * XML firmware configuration file to have type="user".  */
    case gecko_evt_gatt_server_user_write_request_id:
      for (i = 0; i < appCfgGattServerUserWriteRequestSize; i++) {
        if ( (appCfgGattServerUserWriteRequest[i].charId
              == evt->data.evt_gatt_server_characteristic_status.characteristic)
             && (appCfgGattServerUserWriteRequest[i].fctn) ) {
          appCfgGattServerUserWriteRequest[i].fctn(
            &(evt->data.evt_gatt_server_attribute_value.value));
        }
      }
      break;

    /* SW Timer event */
    case gecko_evt_hardware_soft_timer_id:
      switch (evt->data.evt_hardware_soft_timer.handle) {
        case ADV_ALTERNATE_TIMER:
          appBleAdvEventHandler();
          break;

        case IMU_SERVICE_ACC_TIMER:
          imuAccelerationTimerEvtHandler();
          break;

        case IMU_SERVICE_ORI_TIMER:
          imuOrientationTimerEvtHandler();
          break;

        case SENSOR_READOUT_TIMER:
          RADIO_bleStackLoopIteration();
          break;

        case BATT_SERVICE_TIMER:
          battMeasure();
          break;

        default:
          break;
      }
      break;

    /* Discovered device scan response */
    case gecko_evt_le_gap_scan_response_id:
      appCfgScanResponse(evt);
      break;

    case gecko_evt_sm_passkey_display_id:
      break;

    case gecko_evt_sm_passkey_request_id:
      break;

    case gecko_evt_sm_confirm_passkey_id:
      break;

    /* This event is triggered after the bonding procedure has been succesfully completed. */
    case gecko_evt_sm_bonded_id:
      appCfgGattBondedEvent(&(evt->data.evt_sm_bonded));
      break;

    case  gecko_evt_sm_bonding_failed_id:
      break;

    /* Run our default sensor handler code if an interrupt occured. */
    case gecko_evt_system_external_signal_id:
      RADIO_readImuData(true);
      break;
  }
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
