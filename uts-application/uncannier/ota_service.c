///-----------------------------------------------------------------------------
///
/// @file ota_service.c
///
/// @brief OTA DFU service
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include "ota_service.h"
#include <stdbool.h>
#include "bg_types.h"
#include "gatt_db.h"
#include "native_gecko.h"
#include "connection.h"

static bool boot_to_ota;

///-----------------------------------------------------------------------------
///
/// @brief  Initialize the OTA DFU service
///
///-----------------------------------------------------------------------------
void otaServiceInit( void )
{
    boot_to_ota = false;
}

///-----------------------------------------------------------------------------
///
/// @brief  Handle connection closed event
///
///-----------------------------------------------------------------------------
void otaServiceConnectionClosed( void )
{
    if( boot_to_ota )
    {
        // Enter to OTA DFU mode
        gecko_cmd_system_reset( 2 );
    }
}

///-----------------------------------------------------------------------------
///
/// @brief  Handle user write to the control characteristic
/// @param  writeValue  Value being written
///
///-----------------------------------------------------------------------------
void otaServiceControlWrite( uint8array *writeValue )
{
    // Any write triggers a boot to OTA DFU
    boot_to_ota = true;

    // Send response to write request
    gecko_cmd_gatt_server_send_user_write_response( conGetConnectionId(), gattdb_ota_control, bg_err_success );

    // Close connection to enter OTA DFU mode
    gecko_cmd_le_connection_close( conGetConnectionId() );
}
