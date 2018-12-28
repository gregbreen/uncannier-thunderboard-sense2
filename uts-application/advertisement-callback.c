/***************************************************************************//**
 * @file
 * @brief Callbacks related to connectable advertisement
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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* application specific headers */
/*#include "app_ui.h"*/

/* Own header*/
#include "advertisement-callback.h"

/***************************************************************************************************
 * Local Macros and Definitions
 **************************************************************************************************/

/* Text definitions */
#define ADV_ADV_TEXT        "\r\nAdv: M O D E"
#define ADV_NON_ADV_TEXT    "\r\nAdv: Stopped"

/***************************************************************************************************
 * Function Definitions
 **************************************************************************************************/
void advertisingStartedCallback(void)
{
  printf(ADV_ADV_TEXT);

  return;
}

void advertisingStoppedCallback(void)
{
  printf(ADV_NON_ADV_TEXT);
  return;
}
