/***************************************************************************//**
 * @file
 * @brief hallservice.h
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

#ifndef __TBSENSE_GATT_SERVICE_HALL_H_
#define __TBSENSE_GATT_SERVICE_HALL_H_

/* Possible values for State */
#define HALLS_STATE_CLOSED 0
#define HALLS_STATE_OPEN   1
#define HALLS_STATE_TAMPER 2

void hallServiceInit                  (void);
void hallServiceStart                 (void);
void hallServiceStop                  (void);

void hallServiceStateChanged          (uint8_t state);
void hallServiceStateRead             (void);
void hallServiceStateStatusChange     (uint8_t connection, uint16_t clientConfig);

void hallServiceFieldRead             (void);
void hallServiceFieldUpdated          (float field);
void hallServiceFieldStatusChange     (uint8_t connection, uint16_t clientConfig);

void hallServiceReadControlPoint         (void);
void hallServiceControlPointChange       (uint8_t connection, uint16_t clientConfig);
void hallServiceControlPointWrite        (uint8array *writeValue);
void hallServiceControlPointStatusChange (uint8_t connection, uint16_t clientConfig);

#endif
