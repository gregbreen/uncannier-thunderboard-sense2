/***************************************************************************//**
 * @file
 * @brief Callbacks related to connection
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
