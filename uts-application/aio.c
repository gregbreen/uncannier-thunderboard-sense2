#include "aio.h"

#include <stdbool.h>
#include <stdio.h>

/* BG stack headers */
#include "gatt_db.h"
#include "native_gecko.h"
#include "infrastructure.h"

#include "thunderboard/board.h"
#include "ui.h"

#include "connection.h"

/*******************************************************************************
 * Local Macros and Definitions
 ******************************************************************************/

#define AIO_NUMBER_OF_DIGITAL_INPUTS  2 // Implementation supports max 4 inputs
#define AIO_NUMBER_OF_DIGITAL_OUTPUTS 2 // Implementation supports max 4 outputs
#define AIO_DIGITAL_INPUT_PAYLOAD_LEN  (1 + AIO_NUMBER_OF_DIGITAL_INPUTS / 8)
#define AIO_DIGITAL_OUTPUT_PAYLOAD_LEN (1 + AIO_NUMBER_OF_DIGITAL_OUTPUTS / 8)

#define AIO_STATE_NUMBER_OF_BITS      2
#define AIO_STATE_BITS_MASK         0x3

#define AIO_NUMBER_OF_TEST_PINS 10

/** Indicates currently there is no active connection using this service. */
#define AIO_NO_CONNECTION               0xFF

/*******************************************************************************
 * Local Type Definitions
 ******************************************************************************/
typedef enum {
  AIO_DIGITAL_INACTIVE = 0,
  AIO_DIGITAL_ACTIVE   = 1,
  AIO_DIGITAL_TRISTATE = 2,
  AIO_DIGITAL_UNKNOWN  = 3,
} AioDigitalState_t;

/*******************************************************************************
 * Local Variables
 ******************************************************************************/

static bool digitalInNotification = false;
static uint8_t aioLedStates = 0;

/*******************************************************************************
 * Static Function Declarations
 ******************************************************************************/

static void aioDeviceDigitalOutWrite(AioDigitalState_t *aioDigitalOutStates);
static void aioDeviceDigitalInRead(AioDigitalState_t *aioDigitalInStates);

/*******************************************************************************
 * Public Function Definitions
 ******************************************************************************/
void aioInit(void)
{
  return;
}

void aioConnectionOpened(void)
{
  return;
}

void aioConnectionClosed(void)
{
  digitalInNotification = false;
}

void aioDigitalInCharStatusChange(uint8_t connection, uint16_t clientConfig)
{
  printf("AIO: IN Status Change: %d:%04x\r\n", connection, clientConfig);
  /* If the new value of CCC is not 0 (either indication or notification enabled) */
  if (clientConfig) {
    digitalInNotification = true;
  } else {
    digitalInNotification = false;
  }
}

void aioDigitalOutWrite(uint8array *writeValue)
{
  int i;
  AioDigitalState_t aioDigitalOutStates[AIO_NUMBER_OF_DIGITAL_OUTPUTS];

  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_OUTPUTS; i++) {
    aioDigitalOutStates[i] = (AioDigitalState_t)
                             ((writeValue->data[0] >> (AIO_STATE_NUMBER_OF_BITS * i))
                              & AIO_STATE_BITS_MASK);
  }
  aioDeviceDigitalOutWrite(aioDigitalOutStates);

  /* Notify UI service that the RGB LEDs are now disabled */
  uiRGBLEDWasDisabled();

  gecko_cmd_gatt_server_send_user_write_response(conGetConnectionId(),
                                                 gattdb_aio_digital_out,
                                                 0);
}

void aioDigitalOutRead(void)
{
  int i;
  uint8_t ledStates = 0;

  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_OUTPUTS; i++) {
    uint8_t state = (aioLedStates >> i) & 1;
    ledStates |= (state << (i * AIO_STATE_NUMBER_OF_BITS));
  }

  printf("AIO: DO Read: %02x\r\n", ledStates);
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_aio_digital_out,
                                                0,
                                                1,
                                                &ledStates);
}

void aioDigitalInUpdate(void)
{
  int i;
  uint8_t inStates = 0;

  AioDigitalState_t aioDigitalInStates[AIO_NUMBER_OF_DIGITAL_INPUTS];

  aioDeviceDigitalInRead(aioDigitalInStates);

  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_INPUTS; i++) {
    inStates |= aioDigitalInStates[i] << (AIO_STATE_NUMBER_OF_BITS * i);
  }

  // Notify any subscribers
  if (digitalInNotification) {
    gecko_cmd_gatt_server_send_characteristic_notification(
      conGetConnectionId(),
      gattdb_aio_digital_in,
      AIO_DIGITAL_INPUT_PAYLOAD_LEN,
      &inStates);
  }
}

void aioDigitalInRead(void)
{
  int i;
  uint8_t inStates = 0;

  AioDigitalState_t aioDigitalInStates[AIO_NUMBER_OF_DIGITAL_INPUTS];

  aioDeviceDigitalInRead(aioDigitalInStates);

  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_INPUTS; i++) {
    inStates |= aioDigitalInStates[i] << (AIO_STATE_NUMBER_OF_BITS * i);
  }

  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_aio_digital_in,
                                                0,
                                                1,
                                                &inStates);
}

static void aioDeviceDigitalOutWrite(AioDigitalState_t *aioDigitalOutStates)
{
  int i;
  uint8_t ledStates = 0;

  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_OUTPUTS; i++) {
    printf("AIO: DO Write: %d=%d\r\n", i, aioDigitalOutStates[i]);
    switch (aioDigitalOutStates[i]) {
      case AIO_DIGITAL_ACTIVE:
        ledStates |= (1 << i);
        break;

      case AIO_DIGITAL_INACTIVE:
      case AIO_DIGITAL_TRISTATE:
        break;

      default:
        break;
    }
  }

  BOARD_ledSet(ledStates);
  aioLedStates = ledStates;

  return;
}

static void aioDeviceDigitalInRead(AioDigitalState_t *aioDigitalInStates)
{
  uint8_t state = BOARD_pushButtonGetState();
  int i;
  for (i = 0; i < AIO_NUMBER_OF_DIGITAL_INPUTS; i++) {
    uint8_t buttonState = (state >> i) & 1;
    printf("AIO: DI Read: %d=%d\r\n", i, buttonState);
    aioDigitalInStates[i] = buttonState
                            ? AIO_DIGITAL_ACTIVE
                            : AIO_DIGITAL_INACTIVE;
  }
}

void aioLEDWasDisabled(void)
{
  aioLedStates = 0;
}
