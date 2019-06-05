///-----------------------------------------------------------------------------
///
/// @file di_service.c
///
/// @brief Device Information service - custom parts
///
/// @copyright Copyright (c) Uncannier Software 2019
///
///-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include "native_gecko.h"
#include "gatt_db.h"
#include "ci.h"
#include "di_service.h"

#define VER_CHARACTERISTIC_SIZE 20

///-----------------------------------------------------------------------------
///
/// @brief  Initializes the Software Version characteristic with the application version
///
///-----------------------------------------------------------------------------
void diServiceSwVerInit( void )
{
    char software_version[VER_CHARACTERISTIC_SIZE] = { 0 };

    snprintf( software_version, sizeof(software_version) - 1, "%s-%d %07x", CI_VERSION, CI_BUILD, CI_COMMIT );

    gecko_cmd_gatt_server_write_attribute_value( gattdb_software_rev, 0, strlen( software_version ),
                                                 (uint8_t *)software_version );
}

///-----------------------------------------------------------------------------
///
/// @brief  Initializes the Firmware Version characteristic with the stack and bootloader versions
///
/// @param evt_system_boot  Pointer to the Gecko stack boot event
///
///-----------------------------------------------------------------------------
void diServiceFwVerInit( struct gecko_msg_system_boot_evt_t *evt_system_boot )
{
    char firmware_version[40] = { 0 };

    snprintf( firmware_version, sizeof(firmware_version) - 1, "%d.%d.%d-%d %08lx",
              evt_system_boot->major, evt_system_boot->minor, evt_system_boot->patch,
              evt_system_boot->build, evt_system_boot->bootloader );

    // The string will be truncated if it exceeds the maximum characteristic size (unlikely)
    gecko_cmd_gatt_server_write_attribute_value( gattdb_firmware_rev, 0, VER_CHARACTERISTIC_SIZE,
                                                 (uint8_t *)firmware_version );
}
