///-----------------------------------------------------------------------------
///
/// @file si1133.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/si1133.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <si1133.h>
}


uint32_t SI1133_measurementForce( void )
{
    return SI1133_OK;
}

uint32_t SI1133_getMeasurement( float *lux, float *uvi )
{
    *lux = 0;
    *uvi = 0;

    return SI1133_OK;
}

uint32_t SI1133_getIrqStatus( uint8_t *irqStatus )
{
    *irqStatus = 0;

    return SI1133_OK;
}
