///-----------------------------------------------------------------------------
///
/// @file di_service.h
///
/// @brief Device Information service - custom parts
///
/// @copyright Copyright (c) Uncannier Software 2019
///
///-----------------------------------------------------------------------------

#ifndef UNCANNIER_DI_SERVICE_H_
#define UNCANNIER_DI_SERVICE_H_

#include "native_gecko.h"

#ifdef __cplusplus
extern "C" {
#endif

void diServiceSwVerInit( void );
void diServiceFwVerInit( struct gecko_msg_system_boot_evt_t *evt_system_boot );

#ifdef __cplusplus
}
#endif

#endif // UNCANNIER_DI_SERVICE_H_
