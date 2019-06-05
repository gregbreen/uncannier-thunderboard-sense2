///-----------------------------------------------------------------------------
///
/// @file di_service_test.cpp
///
/// @brief Tests for the Device Information service
///
/// @copyright Copyright (c) Uncannier Software 2019
///
///-----------------------------------------------------------------------------

#include <cstdio>
#include <cstdint>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <di_service.h>
#include <ci.h>
#include <gatt_db.h>

TEST_GROUP( di_service )
{
    void setup()
    {
    }

    void teardown()
    {
    }
};

TEST( di_service, SoftwareVersionInit )
{
    char swVersion[20];

    snprintf( swVersion, sizeof( swVersion ), "%s-%d %07x", CI_VERSION, CI_BUILD, CI_COMMIT );

    mock().expectOneCall( "gecko_cmd_gatt_server_write_attribute_value()" )
            .withParameter( "attribute", gattdb_software_rev )
            .withParameter( "offset", 0 )
            .withParameter( "value_len", strlen( swVersion ) )
            .withMemoryBufferParameter( "value_data", (const uint8_t *)swVersion, strlen( swVersion ) );

    diServiceSwVerInit();
}

TEST( di_service, FirmwareVersionInit )
{
    struct gecko_msg_system_boot_evt_t evt_system_boot = { 1, 2, 3, 4, 0xaabbccdd, 0, 0 };
    const char fwVersion[20] = { "1.2.3-4 aabbccdd" };

    mock().expectOneCall( "gecko_cmd_gatt_server_write_attribute_value()" )
            .withParameter( "attribute", gattdb_firmware_rev )
            .withParameter( "offset", 0 )
            .withParameter( "value_len", sizeof( fwVersion ) )
            .withMemoryBufferParameter( "value_data", (const uint8_t *)fwVersion, sizeof( fwVersion ) );

    diServiceFwVerInit( &evt_system_boot );
}
