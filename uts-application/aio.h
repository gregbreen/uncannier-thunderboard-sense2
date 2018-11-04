#ifndef __TBSENSE_GATT_SERVICE_AIO_H_
#define __TBSENSE_GATT_SERVICE_AIO_H_

#include "bg_types.h"

void aioInit(void);
void aioDeInit(void);
void aioConnectionOpened(void);
void aioConnectionClosed(void);
void aioDigitalInCharStatusChange(uint8_t connection, uint16_t clientConfig);
void aioDigitalOutWrite(uint8array *writeValue);
void aioDigitalOutRead(void);
void aioDigitalInRead(void);
void aioDigitalInUpdate(void);

void aioLEDWasDisabled(void);

#endif
