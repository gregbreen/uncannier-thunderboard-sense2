///-----------------------------------------------------------------------------
///
/// @file mic_i2s.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/mic_i2s.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <mic.h>
}


void MIC_start( uint32_t nsamples )
{
}

bool MIC_isBusy( void )
{
    return false;
}

float MIC_getSoundLevel( float *var )
{
    *var = 0;

    return 0;
}
