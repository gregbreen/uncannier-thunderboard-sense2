/***************************************************************************//**
 * @file
 * @brief Application configuration header file
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

#ifndef APP_CFG_H
#define APP_CFG_H

/* BG stack headers */
#include "native_gecko.h"

/***********************************************************************************************//**
 * \defgroup app_cfg Application Configuration
 * \brief Application configuration file.
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app_cfg
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 * Public Macros and Definitions
 **************************************************************************************************/
void appBleInit(void);

/***************************************************************************************************
 * Type Definitions
 **************************************************************************************************/

/** GATT Server Attribute Value Write Configuration.
 *  Structure to register handler functions to characteristic write events. */
typedef struct {
  uint16 charId;                        /**< ID of the Characteristic. */
  void (*fctn)(uint8array *writeValue); /**< Handler function. */
} AppCfgGattServerAttributeValue_t;

/** GATT Server Attribute User Read Configuration.
 *  Structure to register handler functions to user read events. */
typedef struct {
  uint16 charId;                        /**< ID of the Characteristic. */
  void (*fctn)(void); /**< Handler function. */
} AppCfgGattServerUserReadRequest_t;

/** GATT Server Attribute  User Write Configuration.
 *  Structure to register handler functions to user write events. */
typedef struct {
  uint16 charId;                        /**< ID of the Characteristic. */
  /**< Handler function. */
  void (*fctn)(uint8array *writeValue);
} AppCfgGattServerUserWriteRequest_t;

/** GATT Server CCC Status Change Configuration.
 *  Structure to register handler functions to client characteristic configuration status change
 *  events. */
typedef struct {
  uint16 charId;                                            /**< ID of the Characteristic. */
  void (*fctn)(uint8_t connection, uint16_t clientConfig);  /**< Handler function. */
} AppCfgGattServerCharStatus_t;

/** GATT Server Confirmation.
 *  Structure to register handler functions to be called when a confirmation is received. */
typedef struct {
  uint16 charId;                                            /**< ID of the Characteristic. */
  void (*fctn)(uint8_t connection);             /**< Handler function. */
} AppCfgGattServerConfirmation_t;

/***************************************************************************************************
 * Extern Variables
 **************************************************************************************************/

extern AppCfgGattServerAttributeValue_t appCfgGattServerAttributeValue[];
extern AppCfgGattServerUserReadRequest_t appCfgGattServerUserReadRequest[];
extern AppCfgGattServerUserWriteRequest_t appCfgGattServerUserWriteRequest[];
extern AppCfgGattServerConfirmation_t appCfgGattServerConfirmation[];
extern AppCfgGattServerCharStatus_t appCfgGattServerCharStatus[];

extern size_t appCfgGattServerAttributeValueSize;
extern size_t appCfgGattServerUserReadRequestSize;
extern size_t appCfgGattServerUserWriteRequestSize;
extern size_t appCfgGattServerConfirmationSize;
extern size_t appCfgGattServerCharStatusSize;
extern size_t appCfgTimerArraySize;
extern size_t appCfgButtonEventSize;

/***************************************************************************************************
 * Public Function Declarations
 **************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Actions after a connection closed event was generated
 *  \param[in]  connection  Connection ID.
 *  \param[in]  reason  Reason for lost connection.
 **************************************************************************************************/
void appCfgConnectionClosedEvent(uint8_t connection, uint16_t reason);

/***********************************************************************************************//**
 *  \brief  Actions after a reset (system boot-on) event was generated
 **************************************************************************************************/
void appCfgResetEvent(void);

/***********************************************************************************************//**
 *  \brief  Actions after a connection opened event was generated
 *  \param[in]  connection  Connection ID.
 *  \param[in]  bonding  Bonding ID.
 **************************************************************************************************/
void appCfgConnectionOpenedEvent(uint8_t connection, uint8_t bonding);

/***********************************************************************************************//**
 *  \brief  This event indicates that the previous GATT procedure has been completed succesfully or
 *  that is has failed with an error
 *  \param[in]  result  error code of GATT procedure.
 **************************************************************************************************/
void appCfgGattProcCompEvent(uint16_t result);

/***********************************************************************************************//**
 *  \brief  This event indicates that a GATT service at the remote GATT database was discovered.
 *  This event follows the commands issued by the local GATT client
 *  \param[in]  srv Pointer to found service.
 **************************************************************************************************/
void appCfgServiceFoundEvent(struct gecko_msg_gatt_service_evt_t* srv);

/***********************************************************************************************//**
 *  \brief  This event indicates that a GATT characteristic was discovered from a remote GATT
 *  database by gatt_discover_characteristics or gatt_discover_primary_characteristics_by_uuid
 *  command.
 *  \param[in]  chr Pointer to found characteristic.
 **************************************************************************************************/
void appCfgChrFoundEvent(struct gecko_msg_gatt_characteristic_evt_t* chr);

/***********************************************************************************************//**
 *  \brief  This event is triggered after the bonding procedure has been successfully completed.
 *  \param[in]  bond Pointer to bonded event.
 **************************************************************************************************/
void appCfgGattBondedEvent(struct gecko_msg_sm_bonded_evt_t* bond);

/***********************************************************************************************//**
 *  \brief  This event indicates that the value of a characteristic at the remote GATT server was
 *  received.
 *  \param[in]  chr Pointer to changed characteristic value.
 **************************************************************************************************/
void appCfgChrValueReceivedEvent (struct gecko_msg_gatt_characteristic_value_evt_t* chr);

/***********************************************************************************************//**
 *  \brief  This event reports any advertisement or scan response packet that is received by the
 *  module's radio while in scanning mode.
 *  \param[in]  evt Pointer to event received.
 **************************************************************************************************/
void appCfgScanResponse(struct gecko_cmd_packet *evt);

/** @} (end addtogroup app_cfg) */
/** @} (end addtogroup Application) */

#endif /* APP_CFG_H */
