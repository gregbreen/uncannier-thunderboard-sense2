#include "power.h"

/* BG stack headers */
#include "gatt_db.h"
#include "native_gecko.h"

#include "thunderboard/util.h"
#include "connection.h"

void powerInit(void)
{
  return;
}

void powerDeInit(void)
{
  return;
}

void powerSourceTypeRead(void)
{
  uint8_t sourceType = UTIL_supplyGetType();

  gecko_cmd_gatt_server_send_user_read_response(conGetConnectionId(),
                                                gattdb_power_source_type,
                                                0,
                                                1,
                                                &sourceType);
}
