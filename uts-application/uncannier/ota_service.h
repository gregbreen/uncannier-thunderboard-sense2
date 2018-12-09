///-----------------------------------------------------------------------------
///
/// @file ota_service.h
///
/// @brief OTA DFU service
///
/// @copyright Copyright (c) Uncannier Software 2018
///
///-----------------------------------------------------------------------------

#ifndef UNCANNIER_OTA_SERVICE_H_
#define UNCANNIER_OTA_SERVICE_H_

#include "bg_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void otaServiceInit( void );
void otaServiceConnectionClosed( void );
void otaServiceControlWrite( uint8array *writeValue );

#ifdef __cplusplus
}
#endif

#endif // UNCANNIER_OTA_SERVICE_H_
