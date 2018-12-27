/***************************************************************************//**
 * @file
 * @brief radio_ble.h
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

#ifndef __RADIO_BLE_H_
#define __RADIO_BLE_H_

#include <stdint.h>

void RADIO_bleStackInit                (void);
void RADIO_bleStackLoopIteration       (void);
void RADIO_bleChipInit                 (void);
void RADIO_readImuData                 (bool imuIntIsActive);
void RADIO_bleHallStateCallback        (bool pinHigh);
const void *RADIO_bleChipGetDCDCConfig (void);

#endif
