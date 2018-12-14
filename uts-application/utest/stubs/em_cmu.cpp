///-----------------------------------------------------------------------------
///
/// @file em_cmu.cpp
///
/// @brief Stubs for platform/emlib/src/em_cmu.c
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#include <cstdint>
#include <em_cmu.h>


void CMU_ClockEnable( CMU_Clock_TypeDef clock, bool enable )
{
}

void CMU_ClockSelectSet( CMU_Clock_TypeDef clock, CMU_Select_TypeDef ref )
{
}

void CMU_HFXOInit( const CMU_HFXOInit_TypeDef *hfxoInit )
{
}

void CMU_LFXOInit( const CMU_LFXOInit_TypeDef *lfxoInit )
{
}

void CMU_OscillatorEnable( CMU_Osc_TypeDef osc, bool enable, bool wait )
{
}

