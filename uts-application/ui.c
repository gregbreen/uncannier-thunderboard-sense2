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
#include "ui.h"
#include "aio.h"

#include "thunderboard/board.h"
#include "thunderboard/util.h"

#include "radio.h"
#include "radio_ble.h"

uint8_t uiButtonState;
uint8_t uiRGBLEDEnable;

uint8_t uiRGBLEDState[4] = { 0x0, 0x0, 0x50, 0x70 };

void uiInit(void)
{
  uiButtonState  = BOARD_pushButtonGetState();
  uiRGBLEDEnable = 0;

  return;
}

void uiReadButtons(void)
{
  uiButtonState = BOARD_pushButtonGetState();
  printf("UI: Buttons = %X\r\n", uiButtonState);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_ui_buttons,
                                                0,
                                                sizeof(uiButtonState),
                                                (uint8_t *)&uiButtonState);

  return;
}

void uiLEDWrite(uint8array *writeValue)
{
  printf("UI: LED write; %d : %02x\r\n",
         writeValue->len,
         writeValue->data[0]
         );

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_ui_leds,
    UI_WRITE_OK
    );

  return;
}

void uiRGBLEDWrite(uint8array *writeValue)
{
  uint8_t r, g, b;
  uint8_t m;
  uint8_t statusCode;
  uint8_t enableMask;

  printf("UI: RGBLED write; %d : %02x:%02x:%02x:%02x\r\n",
         writeValue->len,
         writeValue->data[0],
         writeValue->data[1],
         writeValue->data[2],
         writeValue->data[3]
         );

  if ( writeValue->len != 4 ) {
    statusCode = UI_WRITE_OK;
  } else {
    m = writeValue->data[0];
    r = writeValue->data[1];
    g = writeValue->data[2];
    b = writeValue->data[3];

    memcpy(uiRGBLEDState, writeValue->data, sizeof(uiRGBLEDState));

    enableMask = m & 0xF;
    if ( UTIL_isLowPower() == false ) {
      BOARD_rgbledEnable(false, ~enableMask);
      BOARD_rgbledEnable(true, enableMask);
      BOARD_rgbledSetColor(r, g, b);

      if ( enableMask == 0 ) {
        /* Prevent the PWM to keep lighting up the small LEDs */
        BOARD_rgbledSetColor(0, 0, 0);
      } else {
        /* Notify AIO service that the LEDs are now disabled */
        aioLEDWasDisabled();
      }
    }

    statusCode = UI_WRITE_OK;
  }

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_ui_rgbleds,
    statusCode
    );

  return;
}

void uiRGBLEDRead(void)
{
  printf("UI: RGBLED read : %02x:%02x:%02x:%02x\r\n",
         uiRGBLEDState[0],
         uiRGBLEDState[1],
         uiRGBLEDState[2],
         uiRGBLEDState[3]
         );

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_ui_rgbleds,
                                                0,
                                                sizeof(uiRGBLEDState),
                                                (uint8_t *)&uiRGBLEDState);

  return;
}

void uiRGBLEDWasDisabled(void)
{
  uiRGBLEDState[0] = 0;

  return;
}
