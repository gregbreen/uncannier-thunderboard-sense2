#ifndef __TBSENSE_GATT_SERVICE_HALL_H_
#define __TBSENSE_GATT_SERVICE_HALL_H_

/* Possible values for State */
#define HALLS_STATE_CLOSED 0
#define HALLS_STATE_OPEN   1
#define HALLS_STATE_TAMPER 2

void hallServiceInit                  (void);
void hallServiceStart                 (void);
void hallServiceStop                  (void);

void hallServiceStateChanged          (uint8_t state);
void hallServiceStateRead             (void);
void hallServiceStateStatusChange     (uint8_t connection, uint16_t clientConfig);

void hallServiceFieldRead             (void);
void hallServiceFieldUpdated          (float field);
void hallServiceFieldStatusChange     (uint8_t connection, uint16_t clientConfig);

void hallServiceReadControlPoint         (void);
void hallServiceControlPointChange       (uint8_t connection, uint16_t clientConfig);
void hallServiceControlPointWrite        (uint8array *writeValue);
void hallServiceControlPointStatusChange (uint8_t connection, uint16_t clientConfig);

#endif
