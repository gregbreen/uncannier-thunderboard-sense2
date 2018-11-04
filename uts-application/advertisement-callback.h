/***********************************************************************************************//**
 * \file   advertisement-callback.h
 * \brief  Callbacks related to connectable advertisement
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
 *  \brief  Advertising started callback.
 *  \details  This function is called by the Application after advertising has been started.
 **************************************************************************************************/
void advertisingStartedCallback(void);

/***********************************************************************************************//**
 *  \brief  Advertising stopped callback.
 *  \details  This function is called by the Application after advertising has been stopped.
 **************************************************************************************************/
void advertisingStoppedCallback(void);
