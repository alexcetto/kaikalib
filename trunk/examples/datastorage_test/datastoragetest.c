//*****************************************************************************
// File Name	: datastoragetest.c
// 
// Title		: example usage of datastorage and pulsecounter functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 08-Jan-2008	Carlos A. Neves		Created the program
//*****************************************************************************

//----- Include Files ---------------------------------------------------------

#include "global.h"		// include our global settings

#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
//#include <avr/signal.h>		// include "signal" names (interrupt names)
#include <avr/interrupt.h>	// include interrupt support
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "uart.h"		// include uart function library
#include "rprintf.h"	        // include printf function library
#include "vt100.h"		// include vt100 terminal support

// kaikalib
#include "df.h"
#include "datastorage.h"	// include datastorage function library
#include "pulsecounter.h"	// include pulse counter function library

// functions
void helpFunction(void);
void dInitFunction(void);
void dStorageFunction(void);
void dRestorageFunction(void);
void dClearFunction(void);
void getFreqFunction(void);
void dataStorageLoop(void);
void dumpParam(void);
void dDumpParamFunction(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
  	_delay_ms(10);

	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	uartSetBaudRate(9600);
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// initialize vt100 terminal
	vt100Init();
	// Init pulse counter
	pulsecounterInit();
	// Init data storage
	dataStorageInit();

	// clear terminal
	vt100ClearScreen();
	vt100SetCursorPos(1,0);
	rprintfProgStrM("\nWelcome to the Data Storage Test Suite!\n");

	// PB2 to blink when a data is acquired
	DDRB |= (1<<PB2);

	// process commands
	dataStorageLoop();

	return 0;
}

void dataStorageLoop(void)
{

  s16 c;

  // print new prompt
  rprintf("\ncmd>");
  
  // testing loop
  while(1)
    {
	  
      // check for keypress
      if((c=uartGetByte()) != -1)
		{
		  switch(c)
			{
			case 'h':
			  helpFunction();
			  break;
			case 'S':
			  dStorageFunction();
			  break;
			case 'R':
			  dRestorageFunction();
			  break;
			case 'C':
			  dClearFunction();
			  break;
			case 'f':
			  getFreqFunction();
			  break;
			case 'd':
			  dDumpParamFunction();
			  break;
			case '\n':
			default:
			  break;
			}
		  // print new prompt
		  rprintf("\ncmd>");
		}
    }
}

void helpFunction(void)
{
	rprintf("\n");
	rprintf("Available commands:\n");
	rprintfProgStrM("h - help\n");
	rprintfProgStrM("S - storage data\n");
	rprintfProgStrM("R - restorage data\n");
	rprintfProgStrM("C - clear data\n");
	rprintfProgStrM("f - frequency\n");
	rprintfProgStrM("d - dump parameters\n");
	rprintf("\n");
}

void dInitFunction(void)
{
  // Init Dataflash
  dataStorageInit();
  myDataStorage.index = 0;
}

void dStorageFunction(void)
{
  u08 status;

  // acquire led on
  PORTB |= (1<<PB2);

  pulsecounterRun();  
  myDataStorage.freq = pulsecounterFrequency();

  myDataStorage.index++;
  myParamStorage.lastIndex = myDataStorage.index;

  rprintf("%d ",
		  myDataStorage.index);
  rprintf("%d\n",
		  myDataStorage.freq);

  // storage datas
  status = dataStorageWrite();
  if (ERR_BUFFER_DIFFERENT_FROM_PAGE == status)
	{
	  rprintfProgStrM("error: buffer different from page.\n");
	}
  // acquire led off
  PORTB &= ~(1<<PB2);
}

void dRestorageFunction(void)
{
  rprintfProgStrM("\number freq\n");

  dataStorageReadInit();
  while (END_OF_DATA != dataStorageReadNext())
    {
	  rprintf("%d ",
			  myDataStorage.index);
	  rprintf("%d\n",
			  myDataStorage.freq);
    }
}

void dClearFunction(void)
{
  rprintfProgStrM("\nClearing data storage...\n");

  myParamStorage.lastIndex = 0;

  myDataStorage.index = 0;
  myDataStorage.freq = 0;

  dataStorageClear();

  rprintfProgStrM("Done.\n");
}

void getFreqFunction(void)
{
  pulsecounterRun();  
  rprintf("\nOscilator: %dHz\n", pulsecounterFrequency());  
}

void dumpParam(void){

  dataStorageParamLoad();

  rprintf("\n");
  rprintf("DF_DENSITY_MBIT = %d\n", DF_DENSITY_MBIT);
  rprintf("DF_PAGES = %d\n", DF_PAGES);
  rprintf("DF_BYTES_PER_PAGE = %d\n", DF_BYTES_PER_PAGE);
  rprintf("paramStorage.pageCounter = %d\n", paramStorage.pageCounter);
  rprintf("paramStorage.byteCounter = %d\n", paramStorage.byteCounter);
  rprintf("paramStorage.flagBusy = %d\n", paramStorage.flagBusy);
  rprintf("paramStorage.flagMemFull = %d\n", paramStorage.flagMemFull);
  rprintf("paramStorage.dataBlocksCounter = %d\n", paramStorage.dataBlocksCounter);
  rprintf("myParamStorage.lastIndex = %d\n", myParamStorage.lastIndex);
}

void dDumpParamFunction(void)
{
  dumpParam();
}

