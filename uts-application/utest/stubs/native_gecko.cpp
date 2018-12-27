///-----------------------------------------------------------------------------
///
/// @file native_gecko.cpp
///
/// @brief Stubs for the Silicon Labs Bluetooth library
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <CppUTestExt/MockSupport.h>
#include "native_gecko.h"

static struct gecko_cmd_packet cmd_msg_buf;
static struct gecko_cmd_packet rsp_msg_buf;

// Horrible global variables
void *gecko_cmd_msg_buf = &cmd_msg_buf;
void *gecko_rsp_msg_buf = &rsp_msg_buf;

static struct gecko_cmd_packet *gecko_rsp_msg = (struct gecko_cmd_packet *)gecko_rsp_msg_buf;

errorcode_t gecko_init( const gecko_configuration_t *config )
{
    return bg_err_success;
}

void gecko_external_signal( uint32 signals )
{
}

void* gecko_cmd_system_reset( uint8 dfu )
{
    mock().actualCall( "gecko_cmd_system_reset()" ).withParameter( "dfu", (int)dfu );

    return 0;
}

struct gecko_msg_le_connection_close_rsp_t* gecko_cmd_le_connection_close( uint8 connection )
{
    mock().actualCall( "gecko_cmd_le_connection_close()" ).withParameter( "connection", connection );

    return &gecko_rsp_msg->data.rsp_le_connection_close;
}

struct gecko_msg_gatt_server_send_user_write_response_rsp_t* gecko_cmd_gatt_server_send_user_write_response( uint8 connection, uint16 characteristic,
                                                                                                             uint8 att_errorcode )
{
    mock().actualCall( "gecko_cmd_gatt_server_send_user_write_response()" )
            .withParameter( "connection", connection )
            .withParameter( "characteristic", characteristic )
            .withParameter( "att_errorcode", att_errorcode );

    return &gecko_rsp_msg->data.rsp_gatt_server_send_user_write_response;
}

struct gecko_msg_gatt_server_send_characteristic_notification_rsp_t* gecko_cmd_gatt_server_send_characteristic_notification( uint8 connection,
                                                                                                                             uint16 characteristic,
                                                                                                                             uint8 value_len,
                                                                                                                             const uint8* value_data )
{
    return &gecko_rsp_msg->data.rsp_gatt_server_send_characteristic_notification;
}

struct gecko_msg_gatt_server_send_user_read_response_rsp_t* gecko_cmd_gatt_server_send_user_read_response( uint8 connection, uint16 characteristic,
                                                                                                           uint8 att_errorcode, uint8 value_len,
                                                                                                           const uint8* value_data )
{
    return &gecko_rsp_msg->data.rsp_gatt_server_send_user_read_response;
}

struct gecko_msg_gatt_server_write_attribute_value_rsp_t* gecko_cmd_gatt_server_write_attribute_value( uint16 attribute, uint16 offset,
                                                                                                       uint8 value_len, const uint8* value_data )
{
    return &gecko_rsp_msg->data.rsp_gatt_server_write_attribute_value;
}

struct gecko_msg_hardware_set_soft_timer_rsp_t* gecko_cmd_hardware_set_soft_timer( uint32 time, uint8 handle, uint8 single_shot )
{
    return &gecko_rsp_msg->data.rsp_hardware_set_soft_timer;
}

struct gecko_msg_le_gap_bt5_set_adv_data_rsp_t* gecko_cmd_le_gap_bt5_set_adv_data( uint8 handle, uint8 scan_rsp, uint8 adv_data_len,
                                                                                   const uint8* adv_data_data )
{
    return &gecko_rsp_msg->data.rsp_le_gap_bt5_set_adv_data;
}

struct gecko_msg_le_gap_start_advertising_rsp_t* gecko_cmd_le_gap_start_advertising( uint8 handle, uint8 discover, uint8 connect )
{
    return &gecko_rsp_msg->data.rsp_le_gap_start_advertising;
}

struct gecko_msg_le_gap_stop_advertising_rsp_t* gecko_cmd_le_gap_stop_advertising( uint8 handle )
{
    return &gecko_rsp_msg->data.rsp_le_gap_stop_advertising;
}

struct gecko_msg_sm_set_bondable_mode_rsp_t* gecko_cmd_sm_set_bondable_mode( uint8 bondable )
{
    return &gecko_rsp_msg->data.rsp_sm_set_bondable_mode;
}

struct gecko_msg_system_get_bt_address_rsp_t* gecko_cmd_system_get_bt_address()
{
    return &gecko_rsp_msg->data.rsp_system_get_bt_address;
}

struct gecko_msg_le_connection_set_parameters_rsp_t* gecko_cmd_le_connection_set_parameters( uint8 connection, uint16 min_interval,
                                                                                             uint16 max_interval, uint16 latency, uint16 timeout )
{
    return &gecko_rsp_msg->data.rsp_le_connection_set_parameters;
}

struct gecko_msg_flash_ps_save_rsp_t* gecko_cmd_flash_ps_save( uint16 key, uint8 value_len, const uint8* value_data )
{
    return &gecko_rsp_msg->data.rsp_flash_ps_save;
}

struct gecko_msg_flash_ps_load_rsp_t* gecko_cmd_flash_ps_load( uint16 key )
{
    return &gecko_rsp_msg->data.rsp_flash_ps_load;
}
