/***********************************************************************************************//**
 * \file   app-callback.h
 * \brief  Callbacks related to Application
 ***************************************************************************************************
 * <b> (C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/***************************************************************************************************
 * Function Declarations
 **************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Reset.
 *  \details  This function is called by the app_cfg plugin after a Reset event was detected.
 **************************************************************************************************/
void resetEventCallback(void);
