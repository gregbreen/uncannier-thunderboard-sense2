/***************************************************************************//**
 * @file
 * @brief imus.h
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

#ifndef __TBSENSE_GATT_SERVICE_IMU_H_
#define __TBSENSE_GATT_SERVICE_IMU_H_

void imuInit                  (void);

void imuConnectionOpened      (void);
void imuConnectionClosed      (void);
void imuServiceStart          (void);
void imuServiceStop           (void);

void imuAccelerationStatusChange    (uint8_t connection, uint16_t clientConfig);
void imuOrientationStatusChange     (uint8_t connection, uint16_t clientConfig);
void imuControlPointStatusChange    (uint8_t connection, uint16_t clientConfig);
bool imuIsAccelerationNotification  (void);
bool imuIsOrientationNotification   (void);

void imuReadControlPoint            (void);
void imuControlPointWrite           (uint8array *writeValue);

void imuAccelerationTimerEvtHandler (void);
void imuOrientationTimerEvtHandler  (void);
void imuDeviceCalibrate             (void);

#endif
