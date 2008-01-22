/*! \file pulsecounter.h \brief Pulse counter function library. */
//*****************************************************************************
//
// File Name	: 'pulsecounter.h'
// Title		: Pulse counter function library
// Author		: Carlos A. Neves - Copyright (C) 2006
// Created		: 2006-08-15
// Revised		: 2006-08-15
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
///	\ingroup driver_avr
/// \defgroup pulsecounter Pulse Counter Function Library (pulsecounter.c)
/// \par Overview
///		This livrary is designed to facilitate the pulse counter to determine
/// the frequency. The signal to be read must be tight to the T1 pin. The Timer/Counter0
/// is used to generate a base time of 1 second.
///
// NOTE: This code is currently below version 1.0, and therefore is considered
// to be lacking in some functionality or documentation, or may not be fully
// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
//@{

#ifndef PULSECOUNTER_H
#define PULSECOUNTER_H

#include "global.h"

// constants/macros/typdefs

#define COUNT_8BITOVERFLOW		56 ///< value for Counter0 overflow

// functions

// Master Pulse Counter Commands

//! Initializes the pulse counter system/library.
void pulsecounterInit(void);

//! Start the pulse counter.
void pulsecounterRun(void);

//! Stop the pulse counter.
void pulsecounterStop(void);

//! Interrupt service routines for pulse counts in T1 pin.
void pulsecounterT1Service(void);

//! Interrupt service routines for Timer/Counter0.
void pulsecounterT0Service(void);

//! Returns the frequency (Hz) in T1 pin.
u16 pulsecounterFrequency(void);

#endif
//@}
