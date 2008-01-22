/*! \file tmp101conf.h \brief TMP100 or TMP101 I2C Digital Temperature Sensor Interface Configuration. */
//*****************************************************************************
//
// File Name	: 'tmp101conf.h'
// Title		: TMP100 or TMP101 I2C Temperature Sensor Interface Configuration
// Author		: Carlos Antonio Neves - Copyright (C) 2007
// Created		: 2007.12.28
// Revised		: 2007.12.28
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef TMP101CONF_H
#define TMP101CONF_H

// Define bus frequency I2C/SMBus in kHz (up to 100kHz)
#define TMP101_BUS_FREQ_KHZ			10

// Define ADC resolution
//  9-Bits -> 0.5oC, 40ms convertion
// 10-Bits -> 0.25oC, 80ms convertion
// 11-Bits -> 0.125oC, 160ms convertion
// 12-Bits -> 0.0625oC, 320ms convertion
#define TMP101_ADC_RESOLUTION			12

#endif
