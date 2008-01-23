/*! \file tmp101.h \brief TMP101 I2C Digital Temperature Sensor header. */
//*****************************************************************************
//
// File Name	: 'tmp101.h'
// Title		: TMP101 I2C Temperature Sensor
// Author		: Carlos Antonio Neves - Copyright (C) 2007
// Created		: 2007.12.28
// Revised		: 2007.12.30
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
///	\ingroup driver_hw
/// \defgroup TMP101 I2C Digital Temperature Sensor and Thermostat Interface (tmp101.h)
/// \code #include "tmp101.h" \endcode
/// \par Overview
///		This library provides high-level functions for accessing the
///		Texas Intruments TMP100 or TMP101 Digital Temperature Sensor
///		with I2C Interface. Use this header with the i2c library.
///
///		The basic specs of the TMP100 and TMP101 are:
///		- Resolution: 9- to 12-Bits, user selectable
///		- Temperature resolution of 0.0625oC
///		- SOT-23-6 package
///		- Single supply operation (2.7V to 5.5V)
///		- I2C compatible (10kHz to 100kHz)
///		- TMP101 offers SMBus alert function
///
//*****************************************************************************
//@{

#ifndef TMP101_H
#define TMP101_H

// Adresses
#define TMP101_UNIT_0       0x90  ///< First TMP100 or TMP101 MODULE I2C address
#define TMP101_UNIT_1       0x92  ///< Second TMP100 or TMP101 MODULE I2C address
#define TMP101_UNIT_2       0x94  ///< Third TMP100 or TMP101 MODULE I2C address
#define TMP101_UNIT_3       0x96  ///< Fourth TMP100 MODULE I2C address
#define TMP101_UNIT_4       0x98  ///< Fifth TMP100 MODULE I2C address
#define TMP101_UNIT_5       0x9A  ///< Sixth TMP100 MODULE I2C address
#define TMP101_UNIT_6       0x9C  ///< Seventh TMP100 MODULE I2C address
#define TMP101_UNIT_7       0x9E  ///< Eighth TMP100 MODULE I2C address

// Command register
#define TMP101_POINT_REG_TEMP_REG     0x00 ///< READ Only
#define TMP101_POINT_REG_CONFIG_REG   0x01 ///< READ/WRITE
#define TMP101_POINT_REG_T_LOW_REG    0x02 ///< READ/WRITE
#define TMP101_POINT_REG_T_HIGH_REG   0x03 ///< READ/WRITE

// Configuration register Bit positions
#define TMP101_CONF_REG_SD		0	///< Shutdown mode
#define TMP101_CONF_REG_TM      1	///< Thermostat mode
#define TMP101_CONF_REG_POL		2	///< Adjust the polarity of ALERT pin output
#define TMP101_CONF_REG_F0		3	///< Flaut Queue bit 0
#define TMP101_CONF_REG_F1		4	///< Flaut Queue bit 1
#define TMP101_CONF_REG_R0		5	///< Converter resolution bit 0
#define TMP101_CONF_REG_R1		6	///< Converter resolution bit 1
#define TMP101_CONF_REG_OS		7	///< One-Shot/Alert

#endif
//@}
