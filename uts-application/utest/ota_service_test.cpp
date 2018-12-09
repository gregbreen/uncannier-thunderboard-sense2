///-----------------------------------------------------------------------------
///
/// @file ota_service_test.cpp
///
/// @brief Tests for the OTA service
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <ota_service.h>
#include <bg_errorcodes.h>
#include <gatt_db.h>

extern "C"
{
#include <connection.h>
}

TEST_GROUP( ota_service )
{
    void setup()
    {
        otaServiceInit();
    }

    void teardown()
    {
    }
};

TEST( ota_service, HandleServiceControlWrite )
{
    mock().expectOneCall( "gecko_cmd_gatt_server_send_user_write_response()" )
            .withParameter( "connection", conGetConnectionId() )
            .withParameter( "characteristic", gattdb_ota_control )
            .withParameter( "att_errorcode", bg_err_success );
    mock().expectOneCall( "gecko_cmd_endpoint_close()" ).withParameter( "endpoint", conGetConnectionId() );

    otaServiceControlWrite( NULL );
}

TEST( ota_service, EnterOtaDfuMode )
{
    mock().expectOneCall( "gecko_cmd_system_reset()" ).withParameter( "dfu", 2 );
    mock().ignoreOtherCalls();

    otaServiceControlWrite( NULL );
    otaServiceConnectionClosed();
}

TEST( ota_service, DoNotEnterOtaDfuModeOnRegularClose )
{
    // The harness will complain if any mocks are called (since no expectations have been set)
    otaServiceConnectionClosed();
}
