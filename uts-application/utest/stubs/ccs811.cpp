///-----------------------------------------------------------------------------
///
/// @file ccs811.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/ccs811.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <ccs811.h>
}


bool CCS811_isDataAvailable( void )
{
    return true;
}

uint32_t CCS811_getMeasurement( uint16_t *eco2, uint16_t *tvoc )
{
    *eco2 = 0;
    *tvoc = 0;

    return CCS811_OK;
}

uint32_t CCS811_setEnvData( int32_t tempData, uint32_t rhData )
{
    return CCS811_OK;
}
