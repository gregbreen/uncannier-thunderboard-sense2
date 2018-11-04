/***********************************************************************************************//**
 * \file   app-callback.c
 * \brief  Callbacks related to Application
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/
#include <stdio.h>

/* Own header*/
#include "app_callback.h"

/***************************************************************************************************
 * Function Definitions
 **************************************************************************************************/
void resetEventCallback(void)
{
  printf("\r\nReset");
  return;
}
