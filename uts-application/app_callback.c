/***************************************************************************//**
 * @file
 * @brief Callbacks related to Application
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
