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
