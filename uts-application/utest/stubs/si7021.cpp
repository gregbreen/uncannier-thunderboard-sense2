///-----------------------------------------------------------------------------
///
/// @file si7021.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/si7021.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <si7021.h>
}


uint32_t SI7021_measure( uint32_t *rhData, int32_t *tData )
{
    *rhData = 0;
    *tData = 0;

    return SI7021_OK;
}
