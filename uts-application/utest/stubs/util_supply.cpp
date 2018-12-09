///-----------------------------------------------------------------------------
///
/// @file util_supply.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/util_supply.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <util.h>
}


void UTIL_shutdown( void )
{
}

uint8_t UTIL_supplyGetType( void )
{
    return UTIL_SUPPLY_TYPE_CR2032;
}

bool UTIL_isLowPower( void )
{
    return true;
}
