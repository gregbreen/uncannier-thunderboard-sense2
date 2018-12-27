/***************************************************************************//**
 * @file
 * @brief app_ble_adv.h
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

#ifndef _APP_BLE_ADV_H_
#define _APP_BLE_ADV_H_

/****************************************************************************/
/* Advertisement Event system config                                        */
/****************************************************************************/
#define APP_ADV_BLE_DATA_ALTERNATE_TIME_MS 1000
#define APP_ADV_BLE_DATA_SWAP_DELAY_MS     1

/****************************************************************************/
/* Advertisement data                                                       */
/****************************************************************************/

#define APP_BLE_ADV_ADV_FLAGS_LENGTH                   2
#define APP_BLE_ADV_ADV_FLAGS_TYPE                     0x01

#define APP_BLE_ADV_MANDATORY_DATA_LENGTH              5
#define APP_BLE_ADV_MANDATORY_DATA_TYPE_MANUFACTURER   0xFF

#define APP_BLE_ADV_COMPANY_ID_SILICON_LABS            0x0047
#define APP_BLE_ADV_FIRMWARE_ID                        0x0002

/** Complete local name. */
#define APP_BLE_ADV_TYPE_LOCAL_NAME                    0x09

/* Bit mask for flags advertising data type. */
#define APP_BLE_ADV_ADV_FLAGS_LE_LIMITED_DISCOVERABLE  0x01
#define APP_BLE_ADV_ADV_FLAGS_LE_GENERAL_DISCOVERABLE  0x02
#define APP_BLE_ADV_ADV_FLAGS_BR_EDR_NOT_SUPPORTED     0x04

#define APP_BLE_ADV_DEVICE_NAME_MAX_LENGTH             20

#define APP_BLE_ADV_DEVICE_NAME_DEFAULT                "Thunder Sense #00000"
#define APP_BLE_ADV_DEVICE_NAME_FORMAT_STRING          "Thunder Sense #%05d"

/****************************************************************************/
/* iBeacon                                                                  */
/****************************************************************************/

#define APP_ADV_BLE_IBEACON_PREAMBLE       0x004C /* Company ID reserved */
#define APP_ADV_BLE_IBEACON_TYPE           0x1502 /* Beacon Type */
#define APP_ADV_BLE_IBEACON_MAJOR          0x0000 /* Beacon major number */
#define APP_ADV_BLE_IBEACON_MINOR          0x0000 /* Beacon minor number */

/** The Beacon's measured RSSI at 1 meter distance in dBm. */
#define APP_ADV_BLE_IBEACON_RSSI           0xC3

/** Length of Manufacturer specific data field. */
#define APP_ADV_BLE_IBEACON_MAN_SPEC_DATA_LEN  26

/** Universally Unique ID used in the Beacon.
 *  128-bit long ID. */
#define APP_ADV_BLE_IBEACON_UUID                  \
  0xCE, 0xF7, 0x97, 0xDA, 0x2E, 0x91, 0x4E, 0xA4, \
  0xA4, 0x24, 0xF4, 0x50, 0x82, 0xAC, 0x06, 0x82

/****************************************************************************/
/* Structure that holds Advertisement data (iBeacon)                        */
/****************************************************************************/
typedef struct __APP_BLE_ADV_iBeaconData{
  uint8_t flagsLength;          /**< Length of the Flags field. */
  uint8_t flagsType;            /**< Type of the Flags field. */
  uint8_t flags;                /**< Flags field. */
  uint8_t mandatoryDataLength;  /**< Length of the Manufacturer Data field. */
  uint8_t mandatoryDataType;    /**< Type of the Manufacturer Data field. */
  uint8_t companyId[2];         /**< Company ID field. */
  uint8_t beaconType[2];        /**< Beacon Type field. */
  uint8_t uuid[16];             /**< 128-bit Universally Unique Identifier. */
  uint8_t major[2];          /**< Beacon major number. */
  uint8_t minor[2];          /**< Beacon minor number. */
  uint8_t rssi;                 /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
} APP_BLE_ADV_iBeaconData;

/****************************************************************************/
/* Structure that holds Scan Response Data                                  */
/****************************************************************************/
typedef struct __APP_BLE_ADV_ScanResponse{
  uint8_t flagsLength;                    /**< Length of the Flags field. */
  uint8_t flagsType;                   /**< Type of the Flags field. */
  uint8_t flags;                       /**< Flags field. */
  uint8_t mandatoryDataLength;                  /**< Length of the mandata field. */
  uint8_t mandatoryDataType;                 /**< Type of the mandata field. */
  uint8_t companyId[2];                   /**< Company ID. */
  uint8_t firmwareId[2];               /**< Firmware ID */
  uint8_t localNameLength;             /**< Length of the local name field. */
  uint8_t localNameType;               /**< Type of the local name field. */
  uint8_t localName[APP_BLE_ADV_DEVICE_NAME_MAX_LENGTH];  /**< Local name field. */
} APP_BLE_ADV_ScanResponse;

void advInit              (void);
void appBleAdvInit        (void);
void appBleAdvSetDevName  (char *devName);
void appBleAdvSetId       (uint32_t uniqueId);
void appBleAdvStart       (void);
void appBleAdvStop        (void);
void appBleAdvEventHandler(void);
bool appBleAdvIsActive    (void);

#endif
