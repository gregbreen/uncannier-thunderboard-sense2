/***************************************************************************//**
 * @file
 * @brief es.h
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

#ifndef __TBSENSE_GATT_SERVICE_ES_H_
#define __TBSENSE_GATT_SERVICE_ES_H_

void esInit                  (void);

void esReadUVIndex           (void);
void esReadPressure          (void);
void esReadTemperature       (void);
void esReadHumidity          (void);
void esReadAmbientLight      (void);
void esReadSoundLevel        (void);

void esReadControlPoint         (void);
void esControlPointChange       (uint8_t connection, uint16_t clientConfig);
void esControlPointWrite        (uint8array *writeValue);
void esControlPointStatusChange (uint8_t connection, uint16_t clientConfig);

#endif
