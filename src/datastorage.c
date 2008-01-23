/*! \file datastorage.c \brief Data storage function library. */
//*****************************************************************************
//
// File Name	: 'datastorage.c'
// Title		: Data storage function library
// Author		: Carlos A. Neves - Copyright (C) 2006
// Created		: 2006-08-30
// Revised		: 2006-09-10
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

// from Procyon AVRLib 
#include "global.h"
#include "timer.h"

// from KAIKA library
#include "df.h"

// include you datastorage struct
#include "datastorage.h"
 
// defines

#define DATA_STORAGE_FIRST_PAGE		1
#define DATA_STORAGE_FIRST_ADDRESS	(sizeof(TParamStorageS) + sizeof(TMyParamStorageS) + 1)
//#define DATA_STORAGE_FIRST_ADDRESS	0

// global variables

u16 gByteCounter;
u16 gPageCounter;

// functions

void dataStorageParamLoad(void)
{
  	dfPageToBufferTransfer(DATA_STORAGE_BUFFER_NUMBER, 
						   0);

	dfBufferRead(DATA_STORAGE_BUFFER_NUMBER, 
				 0, 
				 generalBuffer, 
				 sizeof(TParamStorageS));

	memcpy(&paramStorage, 
		   generalBuffer, 
		   sizeof(TParamStorageS));
  
	dfBufferRead(DATA_STORAGE_BUFFER_NUMBER, 
				 sizeof(TParamStorageS), 
				 generalBuffer, 
				 sizeof(TMyParamStorageS));

	memcpy(&myParamStorage, 
		   generalBuffer, 
		   sizeof(TMyParamStorageS));
}


void dataStorageParamSave(void)
{

  	dfPageToBufferTransfer(DATA_STORAGE_BUFFER_NUMBER, 
						   0);

	memcpy(generalBuffer, 
		   &paramStorage, 
		   sizeof(TParamStorageS));
  
	dfBufferWrite(DATA_STORAGE_BUFFER_NUMBER, 
				  0, 
				  generalBuffer, 
				  sizeof(TParamStorageS));

	memcpy(generalBuffer, 
		   &myParamStorage, 
		   sizeof(TMyParamStorageS));
  
	dfBufferWrite(DATA_STORAGE_BUFFER_NUMBER, 
				  sizeof(TParamStorageS), 
				  generalBuffer, 
				  sizeof(TMyParamStorageS));
  
	dfBufferToMainMemPageProg(DATA_STORAGE_BUFFER_NUMBER, 
							  0, 
							  1);
}

u08 dataStorageInit(void)
{
	// initalize the Dataflash lib
	dfInit();

	// loading the system parameters from Dataflash to memory struct
	dataStorageParamLoad();

	paramStorage.flagBusy = 0;

	// save the parameters into Dataflash
	dataStorageParamSave();

	return ERR_DATASTORAGE_NO_ERROR;
}

u08 dataStorageClear(void)
{
	// reinitialize values of the parameter
	paramStorage.pageCounter = DATA_STORAGE_FIRST_PAGE;
	paramStorage.byteCounter = DATA_STORAGE_FIRST_ADDRESS;
	paramStorage.flagBusy = 0;
	paramStorage.flagMemFull = 0;
	paramStorage.dataBlocksCounter = 0;

	// save the parameters into Dataflash
	dataStorageParamSave();

	return ERR_DATASTORAGE_NO_ERROR;
}

u08 dataStorageWrite(void)
{
  	s16 d;

	paramStorage.flagBusy = 1;
	dataStorageParamSave();

  	// determine how many bytes the dataBuffer is bigger than the page size
	d = DF_BYTES_PER_PAGE - (paramStorage.byteCounter + sizeof(TMyDataStorageS));
  	if (0 > d)
	{
	  	if ((DF_PAGES - 1) == paramStorage.pageCounter)
	    {
			// memory full
			paramStorage.flagMemFull = 1;
			paramStorage.flagBusy = 0;
			dataStorageParamSave();
			return ERR_DATASTORAGE_MEM_FULL;
	    }
	    paramStorage.pageCounter++;
	    paramStorage.byteCounter = 0;
	}
		
	dfPageToBufferTransfer(DATA_STORAGE_BUFFER_NUMBER,
						   paramStorage.pageCounter);

	memcpy(generalBuffer, 
	       &myDataStorage, 
	       sizeof(TMyDataStorageS));
	
	dfBufferWrite(DATA_STORAGE_BUFFER_NUMBER, 
				  paramStorage.byteCounter, 
				  generalBuffer, 
				  sizeof(TMyDataStorageS));
	
	dfBufferToMainMemPageProg(DATA_STORAGE_BUFFER_NUMBER, 
							  paramStorage.pageCounter, 
							  1);
	
  	paramStorage.dataBlocksCounter++;
	paramStorage.byteCounter += sizeof(TMyDataStorageS);

	paramStorage.flagBusy = 0;
	dataStorageParamSave();

	return ERR_DATASTORAGE_NO_ERROR;
}

void dataStorageReadInit(void)
{
  gPageCounter = DATA_STORAGE_FIRST_PAGE;
  gByteCounter = DATA_STORAGE_FIRST_ADDRESS;

  dfPageToBufferTransfer(DATA_STORAGE_BUFFER_NUMBER, 
						 gPageCounter);  
}

u08 dataStorageReadNext(void)
{
  	s16 d;

	if((paramStorage.pageCounter == gPageCounter) &&
	   (paramStorage.byteCounter == gByteCounter))
	{
	  	// end of data
		return END_OF_DATA;
	}

	d = DF_BYTES_PER_PAGE - (gByteCounter + sizeof(TMyDataStorageS));
  	if(0 > d)
	{
	  	if ((DF_PAGES - 1) == gPageCounter)
		{
			// end of memory
		  	return END_OF_MEMORY;
		}
		gByteCounter = 0;
		gPageCounter++;
	}

	dfPageToBufferTransfer(DATA_STORAGE_BUFFER_NUMBER, 
						   gPageCounter);

	dfBufferRead(DATA_STORAGE_BUFFER_NUMBER, 
				 gByteCounter, 
				 generalBuffer, 
				 sizeof(TMyDataStorageS));
	
	memcpy(&myDataStorage, 
		   generalBuffer, 
		   sizeof(TMyDataStorageS));
	
	gByteCounter += sizeof(TMyDataStorageS);

	return ERR_DATASTORAGE_NO_ERROR;
}
