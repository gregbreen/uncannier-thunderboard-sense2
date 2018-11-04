#ifndef __TBSENSE_GATT_SERVICE_UI_H_
#define __TBSENSE_GATT_SERVICE_UI_H_

/* Write response codes*/
#define UI_WRITE_OK                         0
#define UI_ERR_CCCD_CONF                    0x81
#define UI_ERR_PROC_IN_PROGRESS             0x80
#define UI_NO_CONNECTION                    0xFF

void uiInit               (void);
void uiReadButtons        (void);
void uiButtonChange       (uint8_t connection, uint16_t clientConfig);
void uiLEDWrite           (uint8array *writeValue);
void uiRGBLEDWrite        (uint8array *writeValue);
void uiRGBLEDRead         (void);
void uiRGBLEDWasDisabled  (void);
void uiReadControlPoint   (void);
void uiControlPointChange (uint8_t connection, uint16_t clientConfig);
void uiControlPointWrite  (uint8array *writeValue);

#endif
