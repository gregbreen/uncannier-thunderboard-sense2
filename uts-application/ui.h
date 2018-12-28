/***************************************************************************//**
 * @file
 * @brief ui.h
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

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
