/* standard library headers */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* BG stack headers */
#include "bg_types.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "infrastructure.h"

/* plugin headers */
#include "connection.h"

/* Own header*/
#include "iaq.h"

bool iaqNotification;
static uint8_t iaqMeasureMode;
static uint8_t iaqEnable;

extern uint16_t RADIO_eco2;
extern uint16_t RADIO_tvoc;

void iaqInit(void)
{
  iaqNotification = false;

  return;
}

void iaqConnectionClosed(void)
{
  iaqNotification = false;

  return;
}

void iaqConnectionOpened(void)
{
  return;
}

void iaqReadECO2(void)
{
  uint16_t eco2;  /* ppm */

  eco2 = RADIO_eco2;

  printf("IAQ: eCO2 = %d [%Xh]\r\n", eco2, eco2);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_iaq_eco2,
    0,
    sizeof(eco2),
    (uint8_t *)&eco2
    );

  return;
}

void iaqReadTVOC(void)
{
  uint16_t tvoc;  /* ppb */

  tvoc = RADIO_tvoc;

  printf("IAQ: TVOC = %d [%Xh]\r\n", tvoc, tvoc);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_iaq_tvoc,
    0,
    sizeof(tvoc),
    (uint8_t *)&tvoc
    );

  return;
}

void iaqReadControlPoint(void)
{
  uint16_t cp;

  cp = (iaqEnable << 8) | iaqMeasureMode;

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_iaq_control_point,
    0,
    sizeof(cp),
    (uint8_t *)&cp
    );

  return;
}

void iaqControlPointChange(uint8_t connection, uint16_t clientConfig)
{
  uint8_t mode;
  uint8_t enable;

  printf("IAQ_Service: conn = %d data = %04x\r\n", connection, clientConfig);

  mode   = (uint8_t) (clientConfig & 0x03);
  enable = (uint8_t) ((clientConfig >> 15) & 0x01);

  printf("IAQ: enable = %s  mode = %d\r\n",
         (enable == 1) ? "Yes" : "No",
         mode);

  return;
}

/* Write response codes*/
#define IAQ_WRITE_OK                         0
#define IAQ_ERR_CCCD_CONF                    0x81
#define IAQ_ERR_PROC_IN_PROGRESS             0x80
#define IAQ_NO_CONNECTION                    0xFF

void iaqControlPointWrite(uint8array *writeValue)
{
  printf("IAQ_CP: Write; %d : %02x:%02x:%02x:%02x\r\n",
         writeValue->len,
         writeValue->data[0],
         writeValue->data[1],
         writeValue->data[2],
         writeValue->data[3]
         );

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_iaq_control_point,
    IAQ_WRITE_OK
    );

  return;
}

void iaqControlPointStatusChange(uint8_t connection, uint16_t clientConfig)
{
  printf("IAQ_CP_Change: %d:%04x\r\n", connection, clientConfig);

  return;
}
