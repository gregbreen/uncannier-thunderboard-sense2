///-----------------------------------------------------------------------------
///
/// @file imu.cpp
///
/// @brief Stubs for hardware/kit/common/bsp/thunderboard/imu/imu.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>

extern "C"
{
#include <imu.h>
}


uint32_t IMU_init( void )
{
    return IMU_OK;
}

uint32_t IMU_deInit( void )
{
    return IMU_OK;
}

uint8_t IMU_getState( void )
{
    return IMU_STATE_READY;
}

void IMU_update( void )
{
}

void IMU_accelerationGet( int16_t avec[3] )
{
}

void IMU_orientationGet( int16_t ovec[3] )
{
}

uint32_t IMU_gyroCalibrate( void )
{
    return IMU_OK;
}

void IMU_config( float sampleRate )
{
}

bool IMU_isDataReady( void )
{
    return true;
}

bool IMU_isDataReadyFlag( void )
{
    return true;
}

void IMU_clearDataReadyFlag( void )
{
}
