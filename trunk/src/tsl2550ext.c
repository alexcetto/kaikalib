/*! \file tsl2550ext.c \brief TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver Library Extension. */
//*****************************************************************************
//
// File Name	: 'tsl2550ext.c'
// Title		: TAOS TSL2550 Ambient Light Sensor with SMBus Interface Extension
// Author		: Carlos Antonio Neves
// Created		: 2008.01.13
// Revised		: 2008.01.13
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// Part of this code was extracted from 
//    Intelligent Opto Sensor, number 9, Designer's Notebook, 
//    "Simplified TSL2550 Lux Calculation for Embedded and Micro Controllers",
//    contributed by Glenn W. Lee - Revision B - April 11, 2005
//    http://www.taosinc.com/downloads/pdf/dn9b_tsl2550_Lux_Calculation.pdf
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
#include "tsl2550ext.h"

// global variables

// Functions

#ifdef TSL2550_ENABLE_COUNT_VALUE_WITH_REJECTION_FUNCTION

void tsl2550CountValueWithRejection(u08 *value0,
									u08 *value1,
									s16 *countValue0, 
									s16 *countValue1, 
									u08 countToleranceCh0, 
									u08 countToleranceCh1,
									u08 numberOfReadings)
{
  	u08 i;
	s16 cv1Ch0, cv1Ch1;
	s16 cv2Ch0, cv2Ch1;
	s32 cvTotCh0, chTotCh1;

	tsl2550PowerUp();  
	tsl2550Read(value0, value1);
	tsl2550PowerDown();
	cv1Ch0 = tsl2550CountValue(*value0);
	cv1Ch1 = tsl2550CountValue(*value1);

	cvTotCh0 = cv1Ch0;
	chTotCh1 = cv1Ch1;

	for(i=1; i<numberOfReadings; i++)
	{
		tsl2550PowerUp();  
		tsl2550Read(value0, value1);
		tsl2550PowerDown();
		cv2Ch0 = tsl2550CountValue(*value0);
		cv2Ch1 = tsl2550CountValue(*value1);

		if(((cv2Ch0-cv1Ch0) <= countToleranceCh0) && ((cv2Ch1-cv1Ch1) <= countToleranceCh1))
		{
			cvTotCh0 += cv2Ch0;
			chTotCh1 += cv2Ch1;
	    }
		else
		{
			i = 1;
			cvTotCh0 = cv2Ch0;
			chTotCh1 = cv2Ch1;
		}

		cv1Ch0 = cv2Ch0;
		cv1Ch1 = cv2Ch1;

	}
	*countValue0 = (u16)(cvTotCh0/countToleranceCh0);
	*countValue1 = (u16)(chTotCh1/countToleranceCh1);
}

#endif // TSL2550_ENABLE_COUNT_WITH_REJECTION_VALUE_FUNCTION

#ifdef TSL2550_ENABLE_CALCULATE_LUX_FUNCTION

#define MAX_LUX_STD 1846 // standard mode max lux

// Lookup table for channel ratio (i.e. channel1 / channel0)
static u08 ratioLut[129] = {
    100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100,  99,  99,
     99,  99,  99,  99,  99,  99,  99,  99,
     99,  99,  99,  98,  98,  98,  98,  98,
     98,  98,  97,  97,  97,  97,  97,  96,
     96,  96,  96,  95,  95,  95,  94,  94,
     93,  93,  93,  92,  92,  91,  91,  90,
     89,  89,  88,  87,  87,  86,  85,  84,
     83,  82,  81,  80,  79,  78,  77,  75,
     74,  73,  71,  69,  68,  66,  64,  62,
     60,  58,  56,  54,  52,  49,  47,  44,
     42,  41,  40,  40,  39,  39,  38,  38,
     37,  37,  37,  36,  36,  36,  35,  35,
     35,  35,  34,  34,  34,  34,  33,  33,
     33,  33,  32,  32,  32,  32,  32,  31,
     31,  31,  31,  31,  30,  30,  30,  30,
     30
};

// Lookup table to convert channel values to counts
static u16 countLut[128] = {
       0,    1,    2,    3,    4,    5,    6,    7,
       8,    9,   10,   11,   12,   13,   14,   15,
      16,   18,   20,   22,   24,   26,   28,   30,
      32,   34,   36,   38,   40,   42,   44,   46,
      49,   53,   57,   61,   65,   69,   73,   77,
      81,   85,   89,   93,   97,  101,  105,  109,
     115,  123,  131,  139,  147,  155,  163,  171,
     179,  187,  195,  203,  211,  219,  227,  235,
     247,  263,  279,  295,  311,  327,  343,  359,
     375,  391,  407,  423,  439,  455,  471,  487,
     511,  543,  575,  607,  639,  671,  703,  735,
     767,  799,  831,  863,  895,  927,  959,  991,
    1039, 1103, 1167, 1231, 1295, 1359, 1423, 1487,
    1551, 1615, 1679, 1743, 1807, 1871, 1935, 1999,
    2095, 2223, 2351, 2479, 2607, 2735, 2863, 2991,
    3119, 3247, 3375, 3503, 3631, 3759, 3887, 4015
};

// simplified lux equation approximation using lookup tables.
// see http://www.taosinc.com/downloads/pdf/dn9b_tsl2550_Lux_Calculation.pdf
s16 tsl2550CalculateLux(u08 channel0, u08 channel1)
{
    // check for valid bit
    if(!(channel0 & 0x80) || !(channel1 & 0x80)){
    	return -1;
    }
    // get actual 7-bit values
    channel0 &= 0x7F;
    channel1 &= 0x7F;

    // lookup count from channel value
    u16 count0 = countLut[channel0]; // all light
    u16 count1 = countLut[channel1]; // infrared only

    // calculate ratio
    // Note: the "128" is a scaling factor
    u08 ratio = 128; // default

    // avoid division by zero
    // and count1 must not be greater than count0
    if ((count0) && (count1 <= count0)){
        ratio = (count1 * 128 / count0);
    }else{
        return -1;
    }

    // calculate lux
    // Note: the "256" is a scaling factor
    u32 lux = ((count0-count1) * ratioLut[ratio]) / 256;

    // range check lux
    if (lux > MAX_LUX_STD) lux = MAX_LUX_STD;

    return (u16) lux;
}

#endif // TSL2550_ENABLE_CALCULATE_LUX_FUNCTION
