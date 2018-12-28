/***************************************************************************//**
 * @file
 * @brief aio.h
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

#ifndef __TBSENSE_GATT_SERVICE_AIO_H_
#define __TBSENSE_GATT_SERVICE_AIO_H_

#include "bg_types.h"

void aioInit(void);
void aioDeInit(void);
void aioConnectionOpened(void);
void aioConnectionClosed(void);
void aioDigitalInCharStatusChange(uint8_t connection, uint16_t clientConfig);
void aioDigitalOutWrite(uint8array *writeValue);
void aioDigitalOutRead(void);
void aioDigitalInRead(void);
void aioDigitalInUpdate(void);

void aioLEDWasDisabled(void);

#endif
