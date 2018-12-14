///-----------------------------------------------------------------------------
///
/// @file hall_si7210.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/hall_si7210.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <hall.h>
}


uint32_t HALL_measure( uint32_t scale, float *result )
{
    *result = 0;

    return HALL_OK;
}

float HALL_getTamperLevel( void )
{
    return 0;
}
