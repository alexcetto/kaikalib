/*! \file df.c \brief Atmel Dataflash Memory Interface. */
//*****************************************************************************
//
// File Name	: 'df.c'
// Title		: Atmel Dataflash Memory Interface
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.09
// Revised		: 2006.09.10
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

#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

// from Procyon AVRLib
#include "global.h"		// include our global settings
#include "spi.h"		// include spi bus support
#include "rprintf.h"

// from KAIKA lib
#include "df.h"

// include project-specific hardware configuration
//#include "dfconf.h"

// Global variables

// Functions

void dfInit(void)
{
	// initialize SPI interface
	spiInit();

	// some customizations
	// clock = f/128
	cbi(SPCR, SPI2X);
	sbi(SPCR, SPR0);
	sbi(SPCR, SPR1);
	// clock = f/4
/* 	cbi(SPCR, SPI2X); */
/* 	cbi(SPCR, SPR0); */
/* 	cbi(SPCR, SPR1); */

	// select SPI mode 3
	// select clock phase positive-going in middle of data
	sbi(SPCR, CPOL);
	// select phase to rise edge in middle of data
	sbi(SPCR, CPHA);

	// release chip select
	sbi(DF_CS_DDR, DF_CS_PIN);
	sbi(DF_CS_PORT,DF_CS_PIN);
}

void dfMainMemPageReadInit(u16 pageAddr, u16 byteAddr, u08 continuous)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);
	if(continuous)
    {
    	// continous array read command
      	// at the end of page n, it starts at the begin of page n+1
      	spiTransferByte(DF_CMD_MAIN_MEM_PAGE_READ);
    }
	else
    {
    	// read only one page
      	spiTransferByte(DF_CMD_CONT_ARRAY_READ);
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)) + (byteAddr >> 8));
	spiTransferByte((u08)(byteAddr));
	spiTransferByte(0x00);
	spiTransferByte(0x00);
	spiTransferByte(0x00);
	spiTransferByte(0x00);
}

u08 dfMainMemPageReadByte(void)
{
  	return spiTransferByte(0x00);
}

void dfMainMemPageReadStop(void)
{
  	// release chip select
  	sbi(DF_CS_PORT,DF_CS_PIN);  
}

u08 dfStatusRegRead(void)
{
  	u08 statusReg;

  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
  	spiTransferByte(DF_CMD_STATUS_REG_READ);
	statusReg = spiTransferByte(0x00);

	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);
  
	return statusReg;
}

// Buffer operation

void dfBufferRead(u08 bufNum, u16 byteAddr, u08 *myBuffer, u16 numBytes)
{
  	u16 i;

  	// assert chip select
	cbi(DF_CS_PORT,DF_CS_PIN);

	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	spiTransferByte(DF_CMD_BUF_2_READ);
    }
	else
    {
      	spiTransferByte(DF_CMD_BUF_1_READ);
    }
	spiTransferByte(0x00);
	spiTransferByte((u08)(byteAddr>>8));
	spiTransferByte((u08)(byteAddr));
	spiTransferByte(0x00);
	for(i=0; i<numBytes; i++)
    {
      	myBuffer[i] = spiTransferByte(0x00);
    }

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);
}

void dfBufferWrite(u08 bufNum, u16 byteAddr, u08 *myBuffer, u16 numBytes)
{
  	u16 i;

  	// assert chip select
	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	spiTransferByte(DF_CMD_BUF_2_WRITE);
    }
	else
    {
      	spiTransferByte(DF_CMD_BUF_1_WRITE);
    }
	spiTransferByte(0x00);
	spiTransferByte((u08)(byteAddr>>8));
	spiTransferByte((u08)(byteAddr));
	for(i=0; i<numBytes; i++)
    {
      	spiTransferByte(myBuffer[i]);
    }

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);
}

void dfBufferToMainMemPageProg(u08 bufNum, u16 pageAddr, u08 withErase)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
  	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	if(withErase)
		{
	  		spiTransferByte(DF_CMD_BUF_2_TO_MAIN_PAGE_PROG_WITH_ERASE);
		}
		else
		{
		  	spiTransferByte(DF_CMD_BUF_2_TO_MAIN_PAGE_PROG_WITHOUT_ERASE);
		}
    }
	else
    {
      	if(withErase)
		{
		  	spiTransferByte(DF_CMD_BUF_1_TO_MAIN_PAGE_PROG_WITH_ERASE);	  
		}
      	else
		{
	  		spiTransferByte(DF_CMD_BUF_1_TO_MAIN_PAGE_PROG_WITHOUT_ERASE);
		}
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)));
	spiTransferByte(0x00);

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

  	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}

void dfPageToBufferTransfer(u08 bufNum, u16 pageAddr)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	spiTransferByte(DF_CMD_MAIN_MEM_PAGE_TO_BUF_2_TRANS);
    }
	else
    {
      	spiTransferByte(DF_CMD_MAIN_MEM_PAGE_TO_BUF_1_TRANS);
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)));  
	spiTransferByte(0x00);

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

  	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}

u08 dfPageToBufferCompare(u08 bufNum, u16 pageAddr)
{
  	u08 statusReg;

  	// assert chip select
	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	spiTransferByte(DF_CMD_MAIN_MEM_PAGE_TO_BUF_2_COMP);
    }
	else
    {
      	spiTransferByte(DF_CMD_MAIN_MEM_PAGE_TO_BUF_1_COMP);
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)));  
	spiTransferByte(0x00);

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

	// wait until transfer finished
	while(!((statusReg = dfStatusRegRead()) & (1<<DF_RDY)));

	// When the data in the main memory page matches the data in the buffer
	// the function returns 0.
	return (statusReg & (1<<DF_COMP));  
}

void dfAutoPageRewriteThroughBuffer(u08 bufNum, u16 pageAddr)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
    {
      	spiTransferByte(DF_CMD_AUTO_PAGE_REWRITE_THROUGH_BUF_2);
    }
	else
    {
      	spiTransferByte(DF_CMD_AUTO_PAGE_REWRITE_THROUGH_BUF_1);
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)));  
	spiTransferByte(0x00);

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

  	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}

void dfPageErase(u16 pageAddr)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
	spiTransferByte(DF_CMD_PAGE_ERASE);
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)));  
	spiTransferByte(0x00);

  	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}

void dfBlockErase(u16 blockAddr)
{
  	// assert chip select
  	cbi(DF_CS_PORT,DF_CS_PIN);

  	// issue the command
	spiTransferByte(DF_CMD_BLOCK_ERASE);
	spiTransferByte((u08)(blockAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(blockAddr << (DF_PAGE_ADDRESS_BITS - 5)));  
	spiTransferByte(0x00);
	
	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}

void dfMainMemoryPageProgramThroughBuffer(u08 bufNum, u16 pageAddr, u16 byteAddr, u08 *myBuffer, u16 numBytes)
{
  	u16 i;

  	// assert chip select
	cbi(DF_CS_PORT,DF_CS_PIN);

	// issue the command
	if ((2 == bufNum) && (2 == DF_NUMBER_OF_BUFFERS))
	{
      	spiTransferByte(DF_CMD_MAIN_PAGE_PROG_THROUGH_BUF_2);
	}
	else
    {
      	spiTransferByte(DF_CMD_MAIN_PAGE_PROG_THROUGH_BUF_1);
    }
	spiTransferByte((u08)(pageAddr >> (16 - DF_PAGE_ADDRESS_BITS)));
	spiTransferByte((u08)(pageAddr << (DF_PAGE_ADDRESS_BITS - 8)) + (byteAddr >> 8));
	spiTransferByte((u08)(byteAddr));
	for(i=0; i<DF_BYTES_PER_PAGE; i++)
	{
      	spiTransferByte(myBuffer[i]);
	}

	// release chip select
	sbi(DF_CS_PORT,DF_CS_PIN);

	// wait until transfer finished
	while(!(dfStatusRegRead() & (1<<DF_RDY)));
}
