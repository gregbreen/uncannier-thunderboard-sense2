#ifndef __TBSENSE_GATT_SERVICE_ES_H_
#define __TBSENSE_GATT_SERVICE_ES_H_

void esInit                  (void);

void esReadUVIndex           (void);
void esReadPressure          (void);
void esReadTemperature       (void);
void esReadHumidity          (void);
void esReadAmbientLight      (void);
void esReadSoundLevel        (void);

void esReadControlPoint         (void);
void esControlPointChange       (uint8_t connection, uint16_t clientConfig);
void esControlPointWrite        (uint8array *writeValue);
void esControlPointStatusChange (uint8_t connection, uint16_t clientConfig);

#endif
