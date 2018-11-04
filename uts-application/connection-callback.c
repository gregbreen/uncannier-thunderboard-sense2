/***********************************************************************************************//**
 * \file   connection-callback.c
 * \brief  Callbacks related to connection
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* standard library headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Own header*/
#include "connection-callback.h"

#include "app.h"
#include "app_ble_adv.h"

/***************************************************************************************************
 * Local Macros and Definitions
 **************************************************************************************************/

/* Text definitions*/
#define CON_HTMKYFOB_CONN_TEXT       "\r\nConnected\r\n"
#define CON_HTMKYFOB_DISCONN_TEXT    "\r\nDisconnected\r\n"

/***************************************************************************************************
 * Function Definitions
 **************************************************************************************************/
void connectionOpenedEventCallback(void)
{
  printf(CON_HTMKYFOB_CONN_TEXT);
  appBleAdvStop();
  appServiceStart();
  return;
}

void connectionTerminatedEventCallback(void)
{
  printf(CON_HTMKYFOB_DISCONN_TEXT);
  appServiceStop();
  appBleAdvStart();
  return;
}
