///-----------------------------------------------------------------------------
///
/// @file board_4166.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/board_4166.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <board.h>
}


void BOARD_imuClearIRQ( void )
{
}

void BOARD_flashDeepPowerDown( void )
{
}

void BOARD_gasSensorClearIRQ( void )
{
}

void BOARD_alsClearIRQ( void )
{
}

void BOARD_ledSet( uint8_t leds )
{
}

uint8_t BOARD_pushButtonGetState( void )
{
    return 0;
}

void BOARD_rgbledEnable( bool enable, uint8_t mask )
{
}

void BOARD_rgbledSetColor( uint8_t red, uint8_t green, uint8_t blue )
{
}
