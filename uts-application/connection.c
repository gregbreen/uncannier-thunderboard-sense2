/***********************************************************************************************//**
 * \file   connection.c
 * \brief  Functions and data related to connection
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

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
