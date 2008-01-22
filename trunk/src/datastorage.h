/*! \file datastorage.h \brief Data storage function library. */
//*****************************************************************************
//
// File Name	: 'datastorage.h'
// Title		: Data storage function library
// Author		: Carlos A. Neves - Copyright (C) 2006
// Created		: 2006-08-15
// Revised		: 2006-09-10
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
///	\ingroup general
/// \defgroup datastorage Data Storage Function Library (datastorage.c)
/// \par Overview
///		This library is used to storage a data struct into a Dataflash. It
///	depends of Dataflash (df) KAIKA library.
///
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
//@{

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include "global.h"

#include "datastorageconf.h"

// constants/macros/typdefs
typedef struct
{
  u16 pageCounter;
  u16 byteCounter;
  u08 flagBusy;
  u08 flagMemFull;
  u08 bufferNum;
  u32 dataBlocksCounter;
}
TParamStorageS;

// erro codes

#define ERR_DATASTORAGE_NO_ERROR		0 ///< no error
#define ERR_DATASTORAGE_MEM_FULL		1 ///< Dataflash memory full
#define ERR_BUFFER_DIFFERENT_FROM_PAGE		2 ///< buffer is differente from page
#define ERR_MEM_ALLOC				3 ///< can not allocate memory
#define END_OF_MEMORY				4 ///< End of Memory
#define END_OF_DATA				5 ///< End of Data
#define ERR_DEVICE_BUSY				6 ///< Device busy

// variables

TParamStorageS paramStorage;
TMyParamStorageS myParamStorage;
TMyDataStorageS myDataStorage;
u08 generalBuffer[SIZE_OF_GENERAL_BUFFER];

// functions

//! Load parameters
/// Load the parameters from the Dataflash into SRAM
void dataStorageParamLoad(void);

//! Save parameters
/// Save the parameters into the Dataflash from SRAM
void dataStorageParamSave(void);

//! Initializes the data storage library.
/// Returns an error code. Zero for no errors.
u08 dataStorageInit(void);

//! Clear the memory
/// Returns an error code. Zero for no errors.
u08 dataStorageClear(void);

//! Storage the data in the dataStorage into Dataflash
/// Increment the counters after the storage process.
/// Returns an error code. Zero for no errors.
u08 dataStorageWrite(void);

//! Initializes the counter to the read
/// Preparer to sequential read process.
void dataStorageReadInit(void);

//! Read a block of data into the data storage struct
/// Increment the counters after the storage process.
/// Returns an error code. Zero for no errors.
u08 dataStorageReadNext(void);

#endif
//@}
