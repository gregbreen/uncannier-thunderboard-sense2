/* standard library headers */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/* BG stack headers */
#include "bg_types.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "infrastructure.h"

/* plugin headers */
#include "app_timer.h"
#include "connection.h"

#include "thunderboard/imu/imu.h"
#include "thunderboard/icm20648.h"

/* Own header*/
#include "imus.h"

// Measurement periodes in ms.
#define ACCELERATION_MEASUREMENT_PERIOD             200
#define ORIENTATION_MEASUREMENT_PERIOD              200

// Number of axis for acceleration and orientation
#define ACC_AXIS                               3
#define ORI_AXIS                               3

// Acceleration and orientation axis payload length in bytes
#define ACC_AXIS_PAYLOAD_LENGTH                2
#define ORI_AXIS_PAYLOAD_LENGTH                2

// Intertial payload length in bytes
#define ACCELERATION_PAYLOAD_LENGTH  (ACC_AXIS * ACC_AXIS_PAYLOAD_LENGTH)
#define ORIENTATION_PAYLOAD_LENGTH   (ORI_AXIS * ORI_AXIS_PAYLOAD_LENGTH)

#define CP_OPCODE_CALIBRATE             0x01
#define CP_OPCODE_ORIRESET              0x02
#define CP_OPCODE_RESPONSE              0x10
#define CP_OPCODE_CALRESET              0x64

#define CP_RESP_SUCCESS                 0x01
#define CP_RESP_ERROR                   0x02

// Error codes
// Client Characteristic Configuration descriptor improperly configured
#define ERR_CCCD_CONF                   0x81

#define MAX_ACC  1000
#define MIN_ACC -1000

#define MAX_ORI       18000
#define ORI_INCREMENT MAX_ORI / 50

static bool cpIndication = false;
static bool accelerationNotification = false;
static bool orientationNotification  = false;
static bool calibrationInProgress = false;

static void imuSendCalibrateDone  (void);
static void handleDeviceInitDeinit(void);

void imuInit(void)
{
  gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ACC_TIMER, false);
  gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ORI_TIMER, false);

  accelerationNotification = false;
  orientationNotification  = false;

  return;
}

void imuConnectionOpened(void)
{
  return;
}

void imuServiceStop(void)
{
  imuConnectionClosed();
  return;
}

void imuServiceStart(void)
{
  return;
}

bool imuIsAccelerationNotification(void)
{
  return accelerationNotification;
}

bool imuIsOrientationNotification(void)
{
  return orientationNotification;
}

void imuConnectionClosed(void)
{
  gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ACC_TIMER, false);
  gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ORI_TIMER, false);
  accelerationNotification = false;
  orientationNotification  = false;

  return;
}

void imuAccelerationStatusChange(uint8_t connection,
                                 uint16_t clientConfig)
{
  printf("IMU: Acceleration Status Change: %d:%04x\r\n", connection, clientConfig);

  accelerationNotification = (clientConfig > 0);
  handleDeviceInitDeinit();
  if ( accelerationNotification ) {
    gecko_cmd_hardware_set_soft_timer
    (
      TIMER_MS_2_TIMERTICK(ACCELERATION_MEASUREMENT_PERIOD),
      IMU_SERVICE_ACC_TIMER, false
    );
  } else {
    gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ACC_TIMER, false);
  }

  return;
}

void imuOrientationStatusChange(uint8_t connection,
                                uint16_t clientConfig)
{
  printf("IMU: Orientation Status Change: %d:%04x\r\n", connection, clientConfig);

  orientationNotification = (clientConfig > 0);
  handleDeviceInitDeinit();
  if ( orientationNotification ) {
    gecko_cmd_hardware_set_soft_timer
    (
      TIMER_MS_2_TIMERTICK(ORIENTATION_MEASUREMENT_PERIOD),
      IMU_SERVICE_ORI_TIMER, false
    );
  } else {
    gecko_cmd_hardware_set_soft_timer(TIMER_STOP, IMU_SERVICE_ORI_TIMER, false);
  }

  return;
}

void imuAccelerationTimerEvtHandler(void)
{
  if ( calibrationInProgress ) {
    return;
  }

  uint8_t buffer[ACCELERATION_PAYLOAD_LENGTH];
  uint8_t *p;
  int16_t avec[3];

  p = buffer;

  IMU_accelerationGet(avec);
  UINT16_TO_BITSTREAM(p, (uint16_t)avec[0]);
  UINT16_TO_BITSTREAM(p, (uint16_t)avec[1]);
  UINT16_TO_BITSTREAM(p, (uint16_t)avec[2]);

  /*printf("IMU: ACC (%d): %04d,%04d,%04d\r\n", accelerationNotification, avec[0], avec[1], avec[2] );*/
  /*printf("A");*/

  if ( !accelerationNotification ) {
    return;
  }

  gecko_cmd_gatt_server_send_characteristic_notification(
    conGetConnectionId(),
    gattdb_imu_acceleration,
    ACCELERATION_PAYLOAD_LENGTH,
    buffer);

  return;
}

void imuOrientationTimerEvtHandler(void)
{
  if ( calibrationInProgress ) {
    return;
  }

  /* int16_t oriX,oriY,oriZ; */
  uint8_t buffer[ORIENTATION_PAYLOAD_LENGTH];
  uint8_t *p;
  int16_t ovec[3];

  p = buffer;

  IMU_orientationGet(ovec);
  UINT16_TO_BITSTREAM(p, (uint16_t)ovec[0]);
  UINT16_TO_BITSTREAM(p, (uint16_t)ovec[1]);
  UINT16_TO_BITSTREAM(p, (uint16_t)ovec[2]);

  /*printf("IMU: ORI (%d): %04d,%04d,%04d\r\n", orientationNotification, ovec[0], ovec[1], ovec[2] );*/
  /*printf("O");*/

  if (!orientationNotification) {
    return;
  }

  gecko_cmd_gatt_server_send_characteristic_notification(
    conGetConnectionId(),
    gattdb_imu_orientation,
    ORIENTATION_PAYLOAD_LENGTH,
    buffer);

  return;
}

void imuControlPointStatusChange(uint8_t connection, uint16_t clientConfig)
{
  printf("IMU: CP Status Change: %d:%04x\r\n", connection, clientConfig);

  /* Enable / disable indications */
  cpIndication = (clientConfig > 0);

  return;
}

static void imuSendCalibrateDone(void)
{
  uint8_t respBuf[3];
  uint8_t *respBufp;

  if ( cpIndication ) {
    respBufp = respBuf;
    UINT8_TO_BITSTREAM(respBufp, CP_OPCODE_RESPONSE);
    UINT8_TO_BITSTREAM(respBufp, CP_OPCODE_CALIBRATE);
    UINT8_TO_BITSTREAM(respBufp, CP_RESP_SUCCESS);
    gecko_cmd_gatt_server_send_characteristic_notification(conGetConnectionId(),
                                                           gattdb_imu_control_point,
                                                           3,
                                                           respBuf);
  }

  return;
}

void imuDeviceCalibrate(void)
{
  calibrationInProgress = true;

  IMU_gyroCalibrate();

  calibrationInProgress = false;

  imuSendCalibrateDone();

  return;
}

void imuControlPointWrite(uint8array *writeValue)
{
  uint8_t respBuf[3];
  uint8_t *respBufp;

  printf("IMU: CP write; %d : %02x:%02x:%02x:%02x\r\n",
         writeValue->len,
         writeValue->data[0],
         writeValue->data[1],
         writeValue->data[2],
         writeValue->data[3]
         );

  respBufp = respBuf;
  if (cpIndication) {
    gecko_cmd_gatt_server_send_user_write_response(conGetConnectionId(),
                                                   gattdb_imu_control_point,
                                                   0);

    UINT8_TO_BITSTREAM(respBufp, CP_OPCODE_RESPONSE);
    UINT8_TO_BITSTREAM(respBufp, writeValue->data[0]);

    switch (writeValue->data[0]) {
      case CP_OPCODE_CALIBRATE:
        imuDeviceCalibrate();
        break;

      case CP_OPCODE_ORIRESET:
        /* imuDeviceOrientationReset(); */
        UINT8_TO_BITSTREAM(respBufp, CP_RESP_SUCCESS);
        printf("ORIRESET!\r\n");

        gecko_cmd_gatt_server_send_characteristic_notification(conGetConnectionId(),
                                                               gattdb_imu_control_point,
                                                               3,
                                                               respBuf);
        break;

      case CP_OPCODE_CALRESET:
        /* imuDeviceCalibrateReset(); */
        UINT8_TO_BITSTREAM(respBufp, CP_RESP_SUCCESS);

        printf("CALRESET!\r\n");
        gecko_cmd_gatt_server_send_characteristic_notification(conGetConnectionId(),
                                                               gattdb_imu_control_point,
                                                               3,
                                                               respBuf);
        break;

      default:
        UINT8_TO_BITSTREAM(respBufp, CP_RESP_ERROR);
        gecko_cmd_gatt_server_send_characteristic_notification(conGetConnectionId(),
                                                               gattdb_imu_control_point,
                                                               3,
                                                               respBuf);
        break;
    }
  } else {
    gecko_cmd_gatt_server_send_user_write_response(conGetConnectionId(),
                                                   gattdb_imu_control_point,
                                                   ERR_CCCD_CONF);
  }

  return;
}

static void handleDeviceInitDeinit(void)
{
  if ( orientationNotification || accelerationNotification ) {
    if ( IMU_getState() == IMU_STATE_DISABLED ) {
      IMU_init();
      IMU_config(225.0f);
    }
  } else {
    if ( IMU_getState() == IMU_STATE_READY ) {
      IMU_deInit();
    }
  }
}
