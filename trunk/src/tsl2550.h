/*! \file tsl2550.h \brief TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver Library. */
//*****************************************************************************
//
// File Name	: 'tsl2550.h'
// Title		: TAOS TSL2550 Ambient Light Sensor with SMBus
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.06
// Revised		: 2006.08.06
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
/// \defgroup tsl2550 TAOS TSL2550 Ambient Light Sensor with SMBus Interface Driver (tsl2550.c)
/// \code #include "tsl2550.h" \endcode
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
//*****************************************************************************
//@{

#ifndef TSL2550_H
#define TSL2550_H

#include "global.h"
#include "tsl2550conf.h"

// constants/macros/typdefs
#define TSL2550_I2C_ADDR		0x72	///< Base I2C address of TSL2550 devices

// convertion time
#define TSL2550_STANDARD_CONVERTION_TIME_MS		900		///< Standard convertion time (800ms at least)
#define TSL2550_EXTENDED_CONVERTION_TIME_MS		300		///< Extended convertion time ((800/5)ms at least)

// command register defines
#define TSL2550_CMD_POWERDOWN					0x00	///< Power-down state
#define TSL2550_CMD_POWERUP_OR_READ_CMD_REG		0x03	///< Power-up state/Read command register
#define TSL2550_CMD_EXTMODE						0x1d	///< Write command to assert extended range mode
#define TSL2550_CMD_RESET_OR_STDMODE			0x18	///< Write command to reset or return to standard range mode
#define TSL2550_CMD_READCH0						0x43	///< Read ADC channel 0
#define TSL2550_CMD_READCH1						0x83	///< Read ADC channel 1

/// The TSL 2550 contains two ADC registers (channel 0 and channel 1)
// ADC register bits
#define TSL2550_ADCREG_VALID	7	///< ADC channel data is valid. 1 indicates that the ADC has written data into the channel data register, since ADCEN was asserted into the COMMAND register.
#define TSL2550_ADCREG_C2		6	///< CHORD number bit 2
#define TSL2550_ADCREG_C1		5	///< CHORD number bit 1
#define TSL2550_ADCREG_C0		4	///< CHORD number bit 0
#define TSL2550_ADCREG_S3		3	///< STEP number bit 3
#define TSL2550_ADCREG_S2		2	///< STEP number bit 2
#define TSL2550_ADCREG_S1		1	///< STEP number bit 1
#define TSL2550_ADCREG_S0		0	///< STEP number bit 0

// functions

//! Init the TSL2550 chip.
void tsl2550Init(void);

//! Power-up the TSL2550 chip.
void tsl2550PowerUp(void);

//! Power-down the TSL2550 chip.
void tsl2550PowerDown(void);

//! Set Extended Mode
/// The range is extended by 5x.
void tsl2550ExtMode(void);

//! Set Standard Mode
/// Reset device returning to standard mode.
void tsl2550StdMode(void);

//! Read ADC Data Registers of TSL2550 chip and returns by reference.
void tsl2550Read(u08 *channel0, u08 *channel1);

//! Calculate the Count Value of channel
///	Returns:
///	Count value of the channel.
u16 tsl2550CountValue(u08 channel);

#endif
//@}
