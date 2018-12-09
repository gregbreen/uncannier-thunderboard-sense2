///-----------------------------------------------------------------------------
///
/// @file rail_lib.cpp
///
/// @brief Stubs for platform/radio/rail_lib
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <rail.h>
}


RAIL_Status_t RAIL_ConfigPti( RAIL_Handle_t railHandle, const RAIL_PtiConfig_t *ptiConfig )
{
    return RAIL_STATUS_NO_ERROR;
}

RAIL_Status_t RAIL_EnablePti( RAIL_Handle_t railHandle, bool enable )
{
    return RAIL_STATUS_NO_ERROR;
}
