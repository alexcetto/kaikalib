/*! \file tsl2550conf.h \brief TAOS TSL2550 Ambient Light Sensor interface configuration. */
//*****************************************************************************
//
// File Name	: 'tsl2550conf.h'
// Title		: TAOS TSL2550 Ambient Light Sensor interface configuration
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.08
// Revised		: 2006.08.08
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef TSL2550CONF_H
#define TSL2550CONF_H

// define bus frequency I2C/SMBus in kHz (up to 100kHz)
#define TSL2550_BUS_FREQ_KHZ				10

// uncomment to enable these functions
//#define TSL2550_ENABLE_COUNT_VALUE_WITH_REJECTION_FUNCTION
#define TSL2550_ENABLE_CALCULATE_LUX_FUNCTION

#endif
