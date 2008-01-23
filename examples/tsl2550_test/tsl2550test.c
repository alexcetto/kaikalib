//*****************************************************************************
// File Name	: tsl2550test.c
// 
// Title		: example usage of tsl2550 functions
// Revision		: 1.0
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When			Who			Description of change
// -----------	-----------	-----------------------
// 2008-01-13	Carlos A. Neves		Created the program
//
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
#include "tsl2550.h"		// include light sensor function library
#include "tsl2550ext.h"		// include light sensor function library extention

// global variables

// functions
void dataStorageLoop(void);
void helpFunction(void);
void dgetLightFunction(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
  	u16 i;

	// wait 3 seconds
	for(i=0; i<3000; i++)
	{
		_delay_ms(1);
	}

	// PB2 to blink when a data is acquired
	DDRB |= (1<<PB2);

	// initialize our libraries
	// initialize the UART (serial port)
	uartInit();
	uartSetBaudRate(9600);
	// make all rprintf statements use uart for output
	rprintfInit(uartSendByte);
	// initialize vt100 terminal
	vt100Init();
	// Init tsl2550 sensor
	tsl2550Init();
	// clear terminal
	vt100ClearScreen();
	vt100SetCursorPos(1,0);
	rprintfProgStrM("\nWelcome to the KaikaLib Test Suite!\n");

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
			case 'l':
			  dgetLightFunction();
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
	rprintfProgStrM("l - color temperature and lux\n");
	rprintf("\n");
}


void dgetLightFunction(void){

  u08 value0;
  u08 value1;
  s16 countValue0;
  s16 countValue1;
  u16 ctp1000;
  s16 lux;

  // Power-up
  tsl2550PowerUp();
  // read the data convertion
  tsl2550Read(&value0, &value1);
  // Power-down
  tsl2550PowerDown();

  // calcule lux
  lux = tsl2550CalculateLux(value0, value1);

  countValue0 = tsl2550CountValue(value0);
  countValue1 = tsl2550CountValue(value1);

/*   tsl2550CountValueWithRejection(&value0, */
/* 								 &value1, */
/* 								 &countValue0,  */
/* 								 &countValue1,  */
/* 								 5, */
/* 								 5, */
/* 								 3); */

  // calcule color temperature per 1000 
  long c0 = (long)countValue0;
  long c1 = (long)countValue1;
  ctp1000 = (u16)((c1*1000)/(c0));

  rprintfProgStrM("channel0: Vis-IR, channel1: IR\n");
  rprintf("Value0 = 0x%x, Value1 = 0x%x\n", 
		  value0, 
		  value1);
  rprintf("\ntsl2550: color temp per 1000 = %d; lux = %d\n",
		  ctp1000,
		  lux);

  rprintfProgStrM("Color Temperature\n");
  rprintfProgStrM("Fluorescent: < 100\n");
  rprintfProgStrM("Sunglight: 250 to 500\n");
  rprintfProgStrM("Incandecent: > 700\n");

}
