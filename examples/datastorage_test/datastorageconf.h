/*! \file datastorageconf.h \brief Data storage interface configuration. */
//*****************************************************************************
//
// File Name	: 'datastorageconf.h'
// Title		: Data storage interface configuration
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.30
// Revised		: 2006.08.30
// Version		: 0.1
// Target MCU	: Atmel AVR series
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef DATASTORAGECONF_H
#define DATASTORAGECONF_H

// include the Dataflash definitions
#include "dfconf.h"

//! define the buffer will be used
/// Dataflash has one or two buffers
#define DATA_STORAGE_BUFFER_NUMBER	1

//! define the size of general buffer.
/// At least the size of major struct
#define SIZE_OF_GENERAL_BUFFER		10

//! define your parameters struct type
/// It is used for some control of main function
/// Do not change the name of struct.
typedef struct
{
  u32 lastIndex;
}
TMyParamStorageS;

//! define your data struct type
/// Do not change the name of struct. Only the content.
typedef struct
{
  u32 index;
  u16 freq;
}
TMyDataStorageS;

#endif
