/*! \file tsl2550ext.h \brief TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver Library Extension. */
//*****************************************************************************
//
// File Name	: 'tsl2550ext.h'
// Title		: TAOS TSL2550 Ambient Light Sensor with SMBus Extension
// Author		: Carlos Antonio Neves
// Created		: 2008.01.13
// Revised		: 2008.01.13
// Version		: 0.1
// Target MCU	: Atmel AVR Serie
// Editor Tabs	: 4
//
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
///	\ingroup driver_hw
/// \defgroup tsl2550ext TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver Extention (tsl2550ext.c)
/// \code #include "tsl2550ext.h" \endcode
/// \par Overview
///		This library provides high-level functions for accessing the Texas
///		Advanced Optoeletrionic Solutions <http://taosinc.com> TSL2550 
///		Ambient Light Sensor with SMBus Interface.
///
///		The basic specs of the TSL2550 are:
///		- Infrared compensation to approximate human eye response
///		- Companding A/D for wide dynamic range
///		- Rejects 50Hz/60Hz lighting ripple
///		- Single supply operation (2.7V to 5.5V)
///		- I2C compatible (10kHz to 100kHz)
///
///		Part of this code was extracted from 
///		Intelligent Opto Sensor, number 9, Designer's Notebook, 
///		"Simplified TSL2550 Lux Calculation for Embedded and Micro Controllers",
///		contributed by Glenn W. Lee - Revision B - April 11, 2005
///		http://www.taosinc.com/downloads/pdf/dn9b_tsl2550_Lux_Calculation.pdf
///
//*****************************************************************************
//@{

#ifndef TSL2550EXT_H
#define TSL2550EXT_H

#include "global.h"
#include "tsl2550extconf.h"

// constants/macros/typdefs

// functions

#ifdef TSL2550_ENABLE_COUNT_VALUE_WITH_REJECTION_FUNCTION
//! Calculate the Count Values with rejection
void tsl2550CountValueWithRejection(u08 *value0,
									u08 *value1,
									s16 *countValue0, 
									s16 *countValue1, 
									u08 countToleranceCh0, 
									u08 countToleranceCh1,
									u08 numberOfReadings);
#endif

#ifdef TSL2550_ENABLE_CALCULATE_LUX_FUNCTION
//! Calculate the approximated light intensity in Lux
/// see http://www.taosinc.com/downloads/pdf/dn9b_tsl2550_Lux_Calculation.pdf
/// 	Returns:
///	Light intensity in Lux.
s16 tsl2550CalculateLux(u08 channel0, u08 channel1);
#endif

#endif
//@}
