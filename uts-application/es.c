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

#include "thunderboard/mic.h"

/* Own header*/
#include "es.h"

extern uint8_t  RADIO_uvIndex;
extern uint32_t RADIO_ambLight;
extern uint32_t RADIO_rhData;
extern int32_t  RADIO_tempData;
extern float    RADIO_pressure;

extern float    RADIO_soundLevel;
extern uint32_t RADIO_soundLevelBufferCount;
extern float    RADIO_soundLevelAccumulated;

void esInit(void)
{
  return;
}

void esReadUVIndex(void)
{
  printf("ES: UV Index = %d\r\n", RADIO_uvIndex);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_uvindex,
                                                0,
                                                sizeof(RADIO_uvIndex),
                                                (uint8_t *)&RADIO_uvIndex);

  return;
}

void esReadPressure(void)
{
  /* 0.1 Pa */

  /* 1 mbar = 100 Pa. We want 0.1 Pa, so multiply by 1000 */
  uint32_t pressure = (uint32_t)(RADIO_pressure * 1000);

  printf("ES: Pressure = %0.3f mbar\r\n", RADIO_pressure);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_pressure,
                                                0,
                                                sizeof(pressure),
                                                (uint8_t *)&pressure);

  return;
}

extern uint32_t RADIO_rhData;
extern int32_t  RADIO_tempData;
void esReadTemperature(void)
{
  /* 0.01 C */

  int16_t btTemp = RADIO_tempData / 10;

  printf("ES: Temp = %3.2f C\r\n", (float)btTemp / 100.0f);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_temperature,
                                                0,
                                                sizeof(btTemp),
                                                (uint8_t *)&btTemp);

  return;
}

void esReadHumidity(void)
{
  /* 0.01 %RH */
  uint16_t humidity = RADIO_rhData / 10;

  printf("ES: Humidity = %3.2f %%RH\r\n", (float)humidity / 100.0f);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_humidity,
                                                0,
                                                sizeof(humidity),
                                                (uint8_t *)&humidity);

  return;
}

void esReadAmbientLight(void)
{
  /* 0.01 Lux */

  printf("ES: Amb light = %3.2f Lux\r\n", (float)RADIO_ambLight / 100.0f);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_ambient_light,
                                                0,
                                                sizeof(RADIO_ambLight),
                                                (uint8_t *)&RADIO_ambLight);

  return;
}

void esReadSoundLevel(void)
{
  /* 0.01 dBA */
  static int16_t soundLevel = 0;

  soundLevel = (int16_t)(RADIO_soundLevel * 100);
  printf("ES: SoundLevel = %3.2f dBA\r\n", RADIO_soundLevel);

  RADIO_soundLevelAccumulated = 0;
  RADIO_soundLevelBufferCount = 0;

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_es_sound_level,
                                                0,
                                                sizeof(soundLevel),
                                                (uint8_t *)&soundLevel);
  return;
}

void esReadControlPoint(void)
{
  static uint32_t cp = 0;

  cp++;

  printf("ES: CP read; cp = %d\r\n", (int)cp);

  /* Send response to read request */
  gecko_cmd_gatt_server_send_user_read_response(
    conGetConnectionId(),
    gattdb_es_control_point,
    0,
    sizeof(cp),
    (uint8_t *)&cp
    );

  return;
}

void esControlPointChange(uint8_t connection, uint16_t clientConfig)
{
  printf("ES: CP change; conn = %d data = %04x\r\n", connection, clientConfig);
  return;
}

/* Write response codes*/
#define ES_WRITE_OK                         0
#define ES_ERR_CCCD_CONF                    0x81
#define ES_ERR_PROC_IN_PROGRESS             0x80
#define ES_NO_CONNECTION                    0xFF

void esControlPointWrite(uint8array *writeValue)
{
  printf("ES: CP write; %d : %02x:%02x:%02x:%02x\r\n",
         writeValue->len,
         writeValue->data[0],
         writeValue->data[1],
         writeValue->data[2],
         writeValue->data[3]
         );

  gecko_cmd_gatt_server_send_user_write_response(
    conGetConnectionId(),
    gattdb_es_control_point,
    ES_WRITE_OK
    );

  return;
}

void esControlPointStatusChange(uint8_t connection, uint16_t clientConfig)
{
  printf("ES_CP_Change: %d:%04x\r\n", connection, clientConfig);

  return;
}
