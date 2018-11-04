#ifndef __TBSENSE_GATT_SERVICE_IAQ_H_
#define __TBSENSE_GATT_SERVICE_IAQ_H_

#include "bg_types.h"

void iaqInit                  (void);

void iaqReadECO2              (void);
void iaqReadTVOC              (void);
void iaqReadControlPoint      (void);

void iaqControlPointChange    (uint8_t connection, uint16_t clientConfig);
void iaqControlPointWrite     (uint8array *writeValue);
void iaqConnectionClosed(void);
void iaqConnectionOpened(void);
void iaqControlPointStatusChange(uint8_t connection, uint16_t clientConfig);

#endif
