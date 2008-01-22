//*****************************************************************************
// File Name	: kaikatest_tsl.c
// 
// Title		: example usage of tmp101 functions
// Revision		: 0.1
// Notes		:	
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
// 
// Revision History:
// When		Who			Description of change
// -----------	-----------		-----------------------
// 30-Dec-2007	Carlos A. Neves		Created the program
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
#include "i2c.h"

// kaikalib
#include "tmp101.h"		// include tmp101 temperature sensor function library

// defines

// Define bus frequency I2C/SMBus in kHz (up to 400 kHz (fast mode))
// This code does not support the high-speed mode (3.4 MHz)
#define TMP101_BUS_FREQ_KHZ				10

// Define the adc timing resolution
//  9-Bits 0.5oC,     40 ms typical and  75 ms max
// 10-Bits 0.25oC,    80 ms typical and 150 ms max
// 11-Bits 0.125oC,  160 ms typical and 300 ms max
// 12-Bits 0.0625oC, 320 ms typical and 600 ms max
#define TMP101_CONVERSION_TIME_MS		600

// Max and Min temperature
#define TMP101_MAX_TEMP_INT		32
#define TMP101_MAX_TEMP_DEC		0

#define TMP101_MIN_TEMP_INT		31
#define TMP101_MIN_TEMP_DEC		0

// variables
u08 tmp101BufRx[2];
u08 tmp101BufTx[3];
u08 tmp101PointerReg;
u08 c;
u16 i;

// functions
void myTmp101Init(void);
void dataLoop(void);
void helpFunction(void);
void dgetTMP101TempContinous(void);
void dgetTMP101TempContinousLoop(void);
void dgetTMP101TempOneShotWithShutDown(void);
void dsetThermostatMode(void);

//----- Begin Code ------------------------------------------------------------
int main(void)
{
  // initialize our libraries
  // initialize the UART (serial port)
  uartInit();
  uartSetBaudRate(9600);
  // make all rprintf statements use uart for output
  rprintfInit(uartSendByte);
  // initialize vt100 terminal
  vt100Init();
  
  // clear terminal
  vt100ClearScreen();
  vt100SetCursorPos(1,0);
  rprintfProgStrM("\nWelcome to the KAIKA Test Program!\n\n");
  
  /* initialize the tmp101 */
  myTmp101Init();

  // process commands
  dataLoop();
  
  return 0;
}

void myTmp101Init(void)
{

  i2cInit();
  // The TMP100 and TM101 can operate with 100kHz or 
  i2cSetBitrate(TMP101_BUS_FREQ_KHZ);
  
  c = 0x00; // the tmp100 and tmp101 respond to the I2C General Call address (0x00) if the eighth bit is zero.
  // issue a conversation to test chip presence
  // return TRUE if chip detected
  // return FALSE if chip does not respond
  if (i2cMasterSendNI(TMP101_UNIT_0, 1, &c) == I2C_OK)
	{
	  rprintfProgStrM("tmp101: Chip detected.\n");
	}
  else
	{
	  rprintfProgStrM("tmp101: Error! Chip NOT detected.\n");
	}
  
  rprintf("TMP101_CONVERSION_TIME_MS = %d ms\n", TMP101_CONVERSION_TIME_MS);
  
  // Get the Pointer Register
  tmp101PointerReg = TMP101_POINT_REG_CONFIG_REG;
  i2cMasterSend(TMP101_UNIT_0, 1, &tmp101PointerReg);
  i2cMasterReceive(TMP101_UNIT_0, 2, tmp101BufRx);
  rprintf("Old value of TMP101_POINT_REG_CONFIG_REG [1]= 0x%x\n", tmp101BufRx[0]);
  //rprintf("Old value of TMP101_POINT_REG_CONFIG_REG [2]= 0x%x\n", tmp101BufRx[1]); // it doesn't matter
  
  // Set a new configuration
  tmp101BufTx[0] = TMP101_POINT_REG_CONFIG_REG;
  tmp101BufTx[1] = 	(1<<TMP101_CONF_REG_R1) | 
	(1<<TMP101_CONF_REG_R0) | 
	(1<<TMP101_CONF_REG_SD); // Set 12-Bits ADC converter and shutdown mode
  tmp101BufTx[2] = 0;
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx); // Update register pointer
  
  // Get the new configuration
  tmp101PointerReg = TMP101_POINT_REG_CONFIG_REG;
  i2cMasterSend(TMP101_UNIT_0, 1, &tmp101PointerReg);
  i2cMasterReceive(TMP101_UNIT_0, 2, tmp101BufRx);
  rprintf("New value of TMP101_POINT_REG_CONFIG_REG [1]= 0x%x\n", tmp101BufRx[0]);
  //rprintf("New value of TMP101_POINT_REG_CONFIG_REG [2]= 0x%x\n", tmp101BufRx[1]); // it doesn't matter

}

void dataLoop(void)
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
			case 't':
			  dgetTMP101TempContinous();
			  break;
			case 'c':
			  dgetTMP101TempContinousLoop();			  
			  break;
			case 's':
			  dgetTMP101TempOneShotWithShutDown();
			  break;
			case 'm':
			  dsetThermostatMode();			  
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
  rprintfChar('\n');
  rprintfProgStrM("Available commands:\n");
  rprintfProgStrM("h - help\n");
  rprintfProgStrM("t - temperature in continous convertion\n");
  rprintfProgStrM("c - temperature in loop of continous convertion\n");
  rprintfProgStrM("s - One-Shot temperature with shutdown mode\n");
  rprintfProgStrM("m - Set thermostat mode\n");
  rprintfChar('\n');
}

void dgetTMP101TempContinous(void){
  
  rprintfChar('\n');
  
  // Configure to 12-Bits AD convertion and continous convertion (shutdown = 0)
  tmp101BufTx[0] = TMP101_POINT_REG_CONFIG_REG;
  tmp101BufTx[1] = 	(1<<TMP101_CONF_REG_R1) | 
					(1<<TMP101_CONF_REG_R0); // Set 12-Bits ADC converter 
  tmp101BufTx[2] = 0;
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx);  

  // Wait for the convertion
  for (i=0; i<TMP101_CONVERSION_TIME_MS; i++)
	{
	  _delay_ms(1);
	}
  
  // Get the convertion value
  tmp101PointerReg = TMP101_POINT_REG_TEMP_REG;
  i2cMasterSend(TMP101_UNIT_0, 1, &tmp101PointerReg);
  i2cMasterReceive(TMP101_UNIT_0, 2, tmp101BufRx);
  
  // To obtain the decimal part we must do a 4 positions shift
  // and invert the value.  
  rprintfProgStrM("Integer Decimal\n");
  rprintf("%d %d", tmp101BufRx[0], (u08)(tmp101BufRx[1]>>4));

  rprintfChar('\n');

}

void dgetTMP101TempContinousLoop(void){

  rprintfChar('\n');  

  // Configure to 12-Bits AD convertion and continous convertion (ShutDown = 0)
  tmp101BufTx[0] = TMP101_POINT_REG_CONFIG_REG;
  tmp101BufTx[1] = 	(1<<TMP101_CONF_REG_R1) | 
					(1<<TMP101_CONF_REG_R0); // Set 12-Bits ADC converter
  tmp101BufTx[2] = 0;
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx); // Update register pointer
  
  rprintfProgStrM("Integer Decimal\n");

  while(1)
	{		  
	  // Wait for the convertion
	  for (i=0; i<TMP101_CONVERSION_TIME_MS; i++)
		{
		  _delay_ms(1);
		}
	  
	  // Get the convertion value
	  // Initiate the AD convertion
	  tmp101PointerReg = TMP101_POINT_REG_TEMP_REG;
	  i2cMasterSend(TMP101_UNIT_0, 1, &tmp101PointerReg);
	  i2cMasterReceive(TMP101_UNIT_0, 2, tmp101BufRx);
	  
	  // To obtain the decimal part we must do a 4 positions shift
	  // and invert the value.
	  rprintf("%d %d", tmp101BufRx[0], (u08)(tmp101BufRx[1]>>4));
	  
	  rprintfChar('\n');
	  
	}
}

void dgetTMP101TempOneShotWithShutDown(void){
  
  rprintfChar('\n');
  
  // Configure to 12-Bits AD convertion and one-shot convertion (shutdown = 1)
  tmp101BufTx[0] = TMP101_POINT_REG_CONFIG_REG;
  tmp101BufTx[1] = 	(1<<TMP101_CONF_REG_R1) | 
					(1<<TMP101_CONF_REG_R0) |
					(1<<TMP101_CONF_REG_SD) |
					(1<<TMP101_CONF_REG_OS);
  tmp101BufTx[2] = 0;
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx);  

  // Wait for the convertion
  for (i=0; i<TMP101_CONVERSION_TIME_MS; i++)
	{
	  _delay_ms(1);
	}
  
  // Get the convertion value
  tmp101PointerReg = TMP101_POINT_REG_TEMP_REG;
  i2cMasterSend(TMP101_UNIT_0, 1, &tmp101PointerReg);
  i2cMasterReceive(TMP101_UNIT_0, 2, tmp101BufRx);
  
  // To obtain the decimal part we must do a 4 positions shift
  // and invert the value.  
  rprintfProgStrM("Integer Decimal\n");
  rprintf("%d %d", tmp101BufRx[0], (u08)(tmp101BufRx[1]>>4));

  rprintfChar('\n');

}

void dsetThermostatMode(void){

  rprintfChar('\n');

  // Configure to 12-Bits AD convertion, continous convertion (no shutdown), no one-shot,
  // thermostat in comparator mode, pin ALERT gets HIGH when temperature pass the Tmax, and
  // tolerance of 1 consecutive faults.
  // Warning: Don't forget the pull-up resistor at the ALERT pin of TMP101.
  tmp101BufTx[0] = TMP101_POINT_REG_CONFIG_REG;
  tmp101BufTx[1] = 	(1<<TMP101_CONF_REG_R1) | 
					(1<<TMP101_CONF_REG_R0) |
					(0<<TMP101_CONF_REG_SD) |
					(0<<TMP101_CONF_REG_OS) |
					(0<<TMP101_CONF_REG_TM) |
					(1<<TMP101_CONF_REG_POL)|
					(0<<TMP101_CONF_REG_F1) |
					(0<<TMP101_CONF_REG_F0);
  tmp101BufTx[2] = 0;
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx);  

  // Set the limit max
  tmp101BufTx[0] = TMP101_POINT_REG_T_HIGH_REG;
  tmp101BufTx[1] = TMP101_MAX_TEMP_INT;
  tmp101BufTx[2] = (u08)(TMP101_MAX_TEMP_DEC<<4);
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx); // Update register pointer

  // Set the limit min
  tmp101BufTx[0] = TMP101_POINT_REG_T_LOW_REG;
  tmp101BufTx[1] = TMP101_MIN_TEMP_INT;
  tmp101BufTx[2] = (u08)(TMP101_MIN_TEMP_DEC<<4);
  i2cMasterSend(TMP101_UNIT_0, 3, tmp101BufTx); // Update register pointer

  rprintfProgStrM("Thermostat Setting Done.\n");

}
