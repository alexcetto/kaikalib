/*! \file pulsecounter.c \brief Pulse counter function library. */
//*****************************************************************************
//
// File Name	: 'pulsecounter.c'
// Title		: Pulse counter function library
// Author		: Carlos A. Neves - Copyright (C) 2006
// Created		: 2006-08-15
// Revised		: 2006-08-15
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// from Procyon AVRLib 
#include "global.h"
#include "timer.h"

#include "pulsecounter.h"

// variables
volatile unsigned char PulseCounterOverflowFlag;
volatile unsigned char PulseCounterHandleFlag;

// functions

void pulsecounterInit(void)
{
	// try to make sure that timer1 is in "normal" mode
	// most importantly, turn off PWM mode
	timer1PWMOff();

	// attach the pulse service routines
	timerAttach(TIMER0OVERFLOW_INT, pulsecounterT0Service);
	timerAttach(TIMER1OVERFLOW_INT, pulsecounterT1Service);

	// Disconnect Timer/Counter1 from OC1 pin.
	outb(TCCR1A, 0);

	// Init Timer/Counter1
	timer1SetPrescaler(TIMER_CLK_STOP);
	// reset TCNT1
	outb(TCNT1H, 0);
	outb(TCNT1L, 0);
	// enable TCNT1 overflow
	sbi(TIMSK, TOIE1);

	// Init Timer/Counter0
	timer0SetPrescaler(TIMER_CLK_STOP);
	// reset TCNT0
	outb(TCNT0, 0);
	// enable TCNT0 overflow interrupt
	sbi(TIMSK, TOIE0);
}

void pulsecounterRun(void)
{
	// Start pulse counter!
	timer0ClearOverflowCount();

	PulseCounterOverflowFlag = 0;
	PulseCounterHandleFlag = 0;
	
	// clear output compare value A
	outb(OCR1AH, 0);
	outb(OCR1AL, 0);

	// reset TCNT0
	outb(TCNT0, 0);

	// reset TCNT1
	outb(TCNT1H, 0);
	outb(TCNT1L, 0);

  	timer1SetPrescaler(TIMER_CLK_T_RISE);
	timer0SetPrescaler(TIMER_CLK_DIV256);
}

void pulsecounterStop(void)
{
	// disable TCNT1 overflow interrupt
	timer1SetPrescaler(TIMER_CLK_STOP);

	// disable TCNT0 overflow interrupt
	timer0SetPrescaler(TIMER_CLK_STOP);
}

void pulsecounterT1Service(void)
{
	// stop the Timer/Counter1
  	timer1SetPrescaler(TIMER_CLK_STOP);
  	// pulse counter T1 has reached maximum value
  	PulseCounterOverflowFlag = 1;
}

void pulsecounterT0Service(void)
{
	if (timer0GetOverflowCount() == COUNT_8BITOVERFLOW)
	{
		pulsecounterStop();
		PulseCounterHandleFlag = 1;
	}
}

u16 pulsecounterFrequency(void)
{
	volatile u16 PulseCounterFrequency;

	while (!PulseCounterHandleFlag);
	PulseCounterFrequency  = (unsigned short)(TCNT1L<<0);
	PulseCounterFrequency |= (unsigned short)(TCNT1H<<8);

  	return PulseCounterFrequency;
}
