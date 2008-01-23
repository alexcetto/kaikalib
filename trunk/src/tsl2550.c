/*! \file tsl2550.c \brief TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver Library.. */
//*****************************************************************************
//
// File Name	: 'tsl2550.c'
// Title		: TAOS TSL2550 Ambient Light Sensor with SMBus Interface
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.06
// Revised		: 2006.08.29
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

#include "global.h"

#include <avr/io.h>
//#include <avr/signal.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// from Procyon AVRLib
#include "i2c.h"
#include "rprintf.h"

// kaikalib
#include "tsl2550.h"

// global variables
u08 tsl2550RxBuf[1];
u08 tsl2550TxBuf[1];
u16 tsl2550ConvTime = TSL2550_STANDARD_CONVERTION_TIME_MS;

// Functions

void tsl2550Init(void)
{
  	i2cInit();
  	i2cSetBitrate(TSL2550_BUS_FREQ_KHZ);
	tsl2550ConvTime = TSL2550_STANDARD_CONVERTION_TIME_MS;
}

void tsl2550PowerUp(void)
{
	tsl2550TxBuf[0] = TSL2550_CMD_POWERUP_OR_READ_CMD_REG;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
}

void tsl2550PowerDown(void)
{
	tsl2550TxBuf[0] = TSL2550_CMD_POWERDOWN;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
}

void tsl2550ExtMode(void)
{
  	tsl2550TxBuf[0] = TSL2550_CMD_EXTMODE;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
	tsl2550ConvTime = TSL2550_EXTENDED_CONVERTION_TIME_MS;
}

void tsl2550StdMode(void)
{
  	tsl2550TxBuf[0] = TSL2550_CMD_RESET_OR_STDMODE;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
  	tsl2550ConvTime = TSL2550_STANDARD_CONVERTION_TIME_MS;
}

void tsl2550Read(u08 *channel0, u08 *channel1)
{
    u16 i;

	for(i=0;i<tsl2550ConvTime;i++)
	{
	  _delay_ms(1); 
	}

	// channel 0
	tsl2550RxBuf[0] = 0;
	tsl2550TxBuf[0] = TSL2550_CMD_READCH0;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
	i2cMasterReceive(TSL2550_I2C_ADDR, 1, tsl2550RxBuf);
	*channel0 = tsl2550RxBuf[0];

	// channel 1
	tsl2550RxBuf[0] = 0;
	tsl2550TxBuf[0] = TSL2550_CMD_READCH1;
	i2cMasterSend(TSL2550_I2C_ADDR, 1, tsl2550TxBuf);
	i2cMasterReceive(TSL2550_I2C_ADDR, 1, tsl2550RxBuf);
	*channel1 = tsl2550RxBuf[0];
}

static u16 tsl2550CV[8] = {0,16,49,115,247,511,1039,2095}; // chord value
static u08 tsl2550SV[8] = {1,2,4,8,16,32,64,128}; // step value

u16 tsl2550CountValue(u08 channel)
{
	u08 C; // chord number (0 to 7)
	u08 S; // step number (0 to 15)

  	C = (channel & 0x70)>>4;
	S = (channel & 0x0f);
	return (tsl2550CV[C] + tsl2550SV[C] * S);
}

