/***********************************************************************************************//**
 * \file   advertisement-callback.c
 * \brief  Callbacks related to connectable advertisement
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

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
