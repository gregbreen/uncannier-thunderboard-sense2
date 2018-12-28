/***************************************************************************//**
 * @file
 * @brief hallservice.c
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
#include "connection.h"

/* Own header*/
#include "hallservice.h"

#define CP_OPCODE_TAMPER_CLEAR       0x01

static float   magneticField;
static uint8_t hallState;
static bool    stateChangeNotification;
static bool    fieldNotification;
static bool    tamperLatched;

void hallServiceInit(void)
{
  stateChangeNotification = false;
  fieldNotification       = false;
  tamperLatched           = false;

  return;
}

void hallServiceStateChanged(uint8_t state)
{
  const char *stateStr = "";
  switch (state) {
    case HALLS_STATE_CLOSED:
      stateStr = "Closed";
      break;
    case HALLS_STATE_OPEN:
      stateStr = "Open";
      break;
    case HALLS_STATE_TAMPER:
      stateStr = "Tamper";
      break;
  }

  printf("HALL: State CHG %d -> %d (%s)%s\r\n", hallState, state, stateStr, tamperLatched ? " - Tamper latched!" : "");

  hallState = state;

  // Notify any subscribers
  if ( stateChangeNotification && !tamperLatched ) {
    gecko_cmd_gatt_server_send_characteristic_notification(
      conGetConnectionId(),
      gattdb_hall_state,
      sizeof(hallState),
      &hallState);
  }

  if ( hallState == HALLS_STATE_TAMPER ) {
    tamperLatched = true;
  }
}

void hallServiceStateRead(void)
{
  uint8_t state;

  if ( tamperLatched ) {
    state = HALLS_STATE_TAMPER;
  } else {
    state = hallState;
  }

  printf("HALL: State = %u\r\n", state);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_hall_state,
                                                0,
                                                sizeof(state),
                                                &state);
  return;
}

void hallServiceStateStatusChange(uint8_t connection,
                                  uint16_t clientConfig)
{
  printf("HALL: State Status Change: %d:%04x\r\n", connection, clientConfig);

  stateChangeNotification = (clientConfig > 0);

  return;
}

void hallServiceFieldRead(void)
{
  /* 1 uT */

  int32_t magneticFlux;

  /* Flux measured in uT. We want 1 mT. */
  magneticFlux = (int32_t)((magneticField * 1000) + 0.5f);
  printf("HALL: MagneticFlux = %4.3f mT (%ld)\r\n", magneticField, magneticFlux);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_hall_field_strength,
                                                0,
                                                sizeof(magneticFlux),
                                                (uint8_t*)&magneticFlux);
  return;
}

void hallServiceFieldStatusChange(uint8_t connection,
                                  uint16_t clientConfig)
{
  printf("HALL: Field Status Change: %d:%04x\r\n", connection, clientConfig);

  fieldNotification = (clientConfig > 0);

  return;
}

void hallServiceFieldUpdated(float field)
{
  int32_t magneticFlux;

  magneticField = field;
  if ( fieldNotification ) {
    /* Flux measured in uT. We want mT. */
    magneticFlux = (int32_t)((magneticField * 1000) + 0.5f);

    gecko_cmd_gatt_server_send_characteristic_notification(
      conGetConnectionId(),
      gattdb_hall_field_strength,
      sizeof(magneticFlux),
      (uint8_t*)&magneticFlux);
  }
}

void hallServiceReadControlPoint(void)
{
  static uint32_t cp = 0;

  cp++;

  printf("HALL: CP read; cp = %d\r\n", (int)cp);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_hall_control_point,
    0,
    sizeof(cp),
    (uint8_t *)&cp
    );

  return;
}

void hallServiceControlPointChange(uint8_t connection, uint16_t clientConfig)
{
  printf("HALL: CP change; conn = %d data = %04x\r\n", connection, clientConfig);
  return;
}

/* Write response codes*/
#define HALLS_WRITE_OK                         0

void hallServiceControlPointWrite(uint8array *writeValue)
{
  printf("HALL: CP write; %d : %02x:%02x\r\n",
         writeValue->len,
         writeValue->data[0],
         writeValue->data[1]
         );

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_es_control_point,
    HALLS_WRITE_OK
    );

  switch (writeValue->data[0]) {
    case CP_OPCODE_TAMPER_CLEAR:
      tamperLatched = false;
      // Trigger a notification if enabled
      hallServiceStateChanged(hallState);
      break;
  }

  return;
}

void hallServiceControlPointStatusChange(uint8_t connection, uint16_t clientConfig)
{
  printf("HALLS_CP_Change: %d:%04x\r\n", connection, clientConfig);

  return;
}
