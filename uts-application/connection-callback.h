/***********************************************************************************************//**
 * \file   connection-callback.h
 * \brief  Callbacks related to connection
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
 *  \brief  Connection started.
 *  \details  This function is called by the Application after a a connection is made.
 **************************************************************************************************/
void connectionOpenedEventCallback(void);

/***********************************************************************************************//**
 *  \brief  Connection terminated.
 *  \details  This function is called by the Application after a a connection is termninated.
 **************************************************************************************************/
void connectionTerminatedEventCallback(void);
