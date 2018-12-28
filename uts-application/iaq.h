/***************************************************************************//**
 * @file
 * @brief iaq.h
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

#ifndef __TBSENSE_GATT_SERVICE_IAQ_H_
#define __TBSENSE_GATT_SERVICE_IAQ_H_

#include "bg_types.h"

void iaqInit                  (void);

void iaqReadECO2              (void);
void iaqReadTVOC              (void);
void iaqReadControlPoint      (void);

void iaqControlPointChange    (uint8_t connection, uint16_t clientConfig);
void iaqControlPointWrite     (uint8array *writeValue);
void iaqConnectionClosed(void);
void iaqConnectionOpened(void);
void iaqControlPointStatusChange(uint8_t connection, uint16_t clientConfig);

#endif
