/***************************************************************************//**
 * @file
 * @brief Functions and data related to connection
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

/* BG stack headers */
#include "bg_types.h"
#include "native_gecko.h"

#include "connection-callback.h"

/* Own header */
#include "connection.h"

/***********************************************************************************************//**
 * @addtogroup Features
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup connection
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 * Local Macros and Definitions
 **************************************************************************************************/

/** Indicates currently there is no active connection using this service. */
#define CON_NO_CONNECTION        0xFF

/** Indicates currently there is no bonding. */
#define CON_NO_BONDING         0xFF

/***************************************************************************************************
 * Public Variables
 **************************************************************************************************/

/***************************************************************************************************
 * Local Variables
 **************************************************************************************************/

static uint8_t conConnectionId = CON_NO_CONNECTION; /* Connection Handle ID */

/***************************************************************************************************
 * Static Function Declarations
 **************************************************************************************************/

/***************************************************************************************************
 * Function Definitions
 **************************************************************************************************/
void conConnectionInit()
{
#ifdef  SILABS_AF_PLUGIN_CONNECTION_CON_BONDING
  /* Switch on bonding */
  gecko_cmd_sm_set_bondable_mode(1);
#else
  /* Switch off bonding */
  gecko_cmd_sm_set_bondable_mode(0);

#endif
}

void conConnectionStarted(uint8_t connection, uint8_t bonding)
{
  /* Update connection handle ID */
  conConnectionId = connection;

#ifdef SILABS_AF_PLUGIN_CONNECTION_CON_PAIRING
  /* Initiate pairing*/
  if (CON_NO_BONDING == bonding) {
    gecko_cmd_sm_increase_security(connection);
  }

#endif

  connectionOpenedEventCallback(); /* call callback */
}

void conConnectionClosed(void)
{
  conConnectionId = CON_NO_CONNECTION; /* Invalidate connection handle */

  connectionTerminatedEventCallback(); /* call callback */
}

uint8_t conGetConnectionId(void)
{
  /* Return connection handle ID */
  return conConnectionId;
}

/** @} (end addtogroup connection) */
/** @} (end addtogroup Features) */
