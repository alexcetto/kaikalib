/*! \file dfconf.h \brief Atmel Dataflash Memory Interface Configuration. */
//*****************************************************************************
//
// File Name	: 'df.h'
// Title		: Atmel Dataflash Memory Interface Configuration
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.09
// Revised		: 2006.09.10
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

#ifndef DFCONF_H
#define DFCONF_H

// DF card chip select pin defines
#define DF_CS_PORT			PORTB
#define DF_CS_DDR			DDRB
#define DF_CS_PIN			0

// define to zero for Inactive Clock Polarity Low or High
#define DF_SPI_MODE_0_OR_3_ENALBLE

// select code model (see the df.h)
//#define DF_MODEL			9
#define AT45DB161B

#endif
