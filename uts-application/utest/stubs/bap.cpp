///-----------------------------------------------------------------------------
///
/// @file bap.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/bap.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <bap.h>
}


uint32_t BAP_getPressure( float *pressure )
{
    *pressure = 0;

    return BAP_OK;
}
