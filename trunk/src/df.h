/*! \file df.h \brief Atmel Dataflash Memory Interface. */
//*****************************************************************************
//
// File Name	: 'df.h'
// Title		: Atmel Dataflash Memory Interface
// Author		: Carlos Antonio Neves - Copyright (C) 2006
// Created		: 2006.08.09
// Revised		: 2006.09.10
// Version		: 0.1
// Target MCU	: Atmel AVR Series
// Editor Tabs	: 4
//
///	\ingroup driver_hw
/// \defgroup Atmel Dataflash serie Interface (df.c)
/// \code #include "df.h" \endcode
/// \par Description
///		This library offers some simple functions which can be used
///		to read and write data using an Atmel Dataflash memory using
///		pages.
///
/// \par Wiring
///		To work with this library, the memory must be wired to the SPI port of
///		the Atmel microcontroller as described below. Typical Dataflash can
///		operate at up to 20MHz maximum SPI clock rate (thus faster than most
///		AVR's maximum SPI clock rate).
///
/// <pre>
///	SERIAL INPUT (SI):
///		The SI pin is an input-only pin.
///	SERIAL OUTPUT (SO):
///		The SO pin is an output-only.
///	SERIAL CLOCK (SCK):
///		The SCK pin is an input-only. Data is clocked into Dataflash on the 
///		rising edge of SCK and clocked out of Dataflash on the falling edge
///		 of SCK.
///	CHIP SELECT (/CS):
///		Active in low (*).
///	WRITE PROTECT (/WP):
///		Active in low. Wire to VCC to enable write mode.
///	RESET (/RESET)
///		Active in low. The Dataflash incorporates an internal power-on reset 
///		circuit, so there are no restrictions on the /RESET pin during 
///		power-on sequences. If this feature is not used, wire this pin to VCC.
///	READY|/BUSY (READY) (not used in this library):
///		This open drain output pin will be driven low when the dataflash is busy
///		in an internally self-timed operation. This pin is normally in a high 
///		state (through a 1 kohm external pull-up resistor), will be pulled low
///		during programming operations, compare operations, and during 
///		page-to-buffer transfers. Instead of use this pin, it can be replaced by
///		monitoring the bit 7 (RDY) of status register.
///	VCC:
///		Wire to power (2.5 V or 2.7 V up to 3.6 V).
///	GND:
///		Wire to ground.
///
///	    (*) you must define this chip select I/O pin in dfconf.h
/// </pre>
/// \note This code is currently below version 1.0, and therefore is considered
/// to be lacking in some functionality or documentation, or may not be fully
/// tested.  Nonetheless, you can expect most functions to work.
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************
//@{

#ifndef DF_H
#define DF_H

#include "global.h"
#include "dfconf.h"

#ifdef AT45DB011B
#define DF_DENSITY_MBIT			1
#define DF_PAGES				512
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	1
#endif

#ifdef AT45DB011D
#define DF_DENSITY_MBIT			1
#define DF_PAGES				512
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	1
#endif

#ifdef AT45DB021B
#define DF_DENSITY_MBIT			2
#define DF_PAGES				1024
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB021D
#define DF_DENSITY_MBIT			2
#define DF_PAGES				1024
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	1
#endif

#ifdef AT45DB041B
#define DF_DENSITY_MBIT			4
#define DF_PAGES				2048
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB041D
#define DF_DENSITY_MBIT			4
#define DF_PAGES				2048
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB081B
#define DF_DENSITY_MBIT			8
#define DF_PAGES				4096
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef  AT45DB081D
#define DF_DENSITY_MBIT			8
#define DF_PAGES				4096
#define DF_BYTES_PER_PAGE		264
#define DF_PAGE_ADDRESS_BITS	9
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB161B
#define DF_DENSITY_MBIT			16
#define DF_PAGES				4096
#define DF_BYTES_PER_PAGE		528
#define DF_PAGE_ADDRESS_BITS	10
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB161D
#define DF_DENSITY_MBIT			16
#define DF_PAGES				4096
#define DF_BYTES_PER_PAGE		528
#define DF_PAGE_ADDRESS_BITS	10
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB321C
#define DF_DENSITY_MBIT			32
#define DF_PAGES				8192
#define DF_BYTES_PER_PAGE		528
#define DF_PAGE_ADDRESS_BITS	10
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB321D
#define DF_DENSITY_MBIT			32
#define DF_PAGES				8192
#define DF_BYTES_PER_PAGE		528
#define DF_PAGE_ADDRESS_BITS	10
#define DF_NUMBER_OF_BUFFERS	2
#endif

#ifdef AT45DB642D
#define DF_DENSITY_MBIT			64
#define DF_PAGES				8192
#define DF_BYTES_PER_PAGE		1056
#define DF_PAGE_ADDRESS_BITS	11
#define DF_NUMBER_OF_BUFFERS	2
#endif

// Status Register Format

// Bit Status Register
#define DF_RDY		7	///< Ready status. Low when is ready to accept commands.
#define DF_COMP		6	///< Compare Buffer with Page. Low when data in the main memory page matches the data in the buffer 

// Commands

// Read Commands
#ifdef DF_SPI_MODE_0_OR_3_ENALBLE
#define DF_CMD_CONT_ARRAY_READ			0xe8 ///< Continuous Array Read
#define DF_CMD_MAIN_MEM_PAGE_READ		0xd2 ///< Main memory page read
#define DF_CMD_BUF_1_READ				0xd4 ///< Buffer 1 read
#define DF_CMD_BUF_2_READ				0xd6 ///< Buffer 2 read
#define DF_CMD_STATUS_REG_READ			0xd7 ///< Status register read
#else
#define DF_CMD_CONT_ARRAY_READ			0x68 ///< Continuous Array Read
#define DF_CMD_MAIN_MEM_PAGE_READ		0x52 ///< Main memory page read
#define DF_CMD_BUF_1_READ				0x54 ///< Buffer 1 read
#define DF_CMD_BUF_2_READ				0x56 ///< Buffer 2 read
#define DF_CMD_STATUS_REG_READ			0x57 // Status register read
#endif

// Program and Erase Commands
#define DF_CMD_BUF_1_WRITE		0x84 ///< Buffer 1 write
#define DF_CMD_BUF_2_WRITE		0x87 ///< Buffer 2 write
#define DF_CMD_BUF_1_TO_MAIN_PAGE_PROG_WITH_ERASE		0x83 ///< Buffer 1 to main memory page program with built-in erase
#define DF_CMD_BUF_2_TO_MAIN_PAGE_PROG_WITH_ERASE		0x86 ///< Buffer 2 to main memory page program with built-in erase
#define DF_CMD_BUF_1_TO_MAIN_PAGE_PROG_WITHOUT_ERASE	0x88 ///< Buffer 1 to main memory page program without built-in erase
#define DF_CMD_BUF_2_TO_MAIN_PAGE_PROG_WITHOUT_ERASE	0x89 ///< Buffer 2 to main memory page program without built-in erase
#define DF_CMD_PAGE_ERASE		0x81 ///< Page erase
#define DF_CMD_BLOCK_ERASE		0x50 ///< Block erase. A block is a group of 8 pages
#define DF_CMD_MAIN_PAGE_PROG_THROUGH_BUF_1		0x82 ///< Main memory page program through buffer 1
#define DF_CMD_MAIN_PAGE_PROG_THROUGH_BUF_2		0x85 ///< Main memory page program through buffer 2

// Additional Commands
#define DF_CMD_MAIN_MEM_PAGE_TO_BUF_1_TRANS		0x53 ///< Main memory page to buffer 1 transfer
#define DF_CMD_MAIN_MEM_PAGE_TO_BUF_2_TRANS		0x55 ///< Main memory page to buffer 2 transfer
#define DF_CMD_MAIN_MEM_PAGE_TO_BUF_1_COMP		0x60 ///< Main memory page to buffer 1 compare
#define DF_CMD_MAIN_MEM_PAGE_TO_BUF_2_COMP		0x61 ///< Main memory page to buffer 2 compare
#define DF_CMD_AUTO_PAGE_REWRITE_THROUGH_BUF_1		0x58 ///< Auto page rewrite through buffer 1
#define DF_CMD_AUTO_PAGE_REWRITE_THROUGH_BUF_2		0x59 ///< Auto page rewrite through buffer 2

// Some explanations were extracted from the Atmel Dataflash datasheet for
// an easy comprehension of functions.

// functions
//! Initialize AVR<->Dataflash hardware interface.
/// Prepares hardware for Dataflash access.
void dfInit(void);

//! Initialize main memory page read
/// A main memory page allows the user to read data directly from any
/// page in the main memory, bypassing both of the data buffers and
/// leaving the contents of the buffers unchanged.
/// By suppling an initial staring address for the main memory array,
/// the continuous flag can be utilized to sequentially read a continuous
/// stream of data from the device. The internal page counter increments
/// automatically. At the last page, the counter returns to the first page.
/// When the continuous flag is 0, only one page is read.
void dfMainMemPageReadInit(u16 pageAddr, u16 byteAddr, u08 continuous);

//! Read a byte from main memory page
/// Read a single byte directly from the main memory.
u08 dfMainMemPageReadByte(void);

//! Stop reading from main memory page
/// Stop the reading process directly from the main memory.
void dfMainMemPageReadStop(void);

//! Read status register
/// The status register can be used to determine the device's
/// Ready/Busy status, the result of a Main Memory Page to Buffer
/// Compare operation, or the device density.
u08 dfStatusRegRead(void);

//! Read from the buffer to AVR memory RAM
/// Data can be read from either on of the two buffers, using different
/// opcodes to specify which buffer to read from into an AVR bufferm RAM.
void dfBufferRead(u08 bufNum, u16 byteAddr, u08 *myBuffer, u16 numBytes);

//! Write to the buffer from AVR memory RAM
/// Data can be shifted in from the SO pin into either buffer 1 or buffer 2
/// from an AVR buffer RAM.
void dfBufferWrite(u08 bufNum, u16 byteAddr, u08 *myBuffer, u16 numBytes);

//! Buffer to main memory page program
/// Data written into either buffer 1 or buffer 2 can be programmed into
/// the main memory with or without built-in erase.
/// Successive page programming operations without doing a page erase are
/// not recommended. In other words, changing bytes within a page from a 
/// "1" to a "0" during multiple page programming operations without
/// erasing that page is not recommended.
//void dfBufferToMainMemPageProg(u08 bufNum, u16 pageAddr, u16 byteAddr, u08 withErase);
void dfBufferToMainMemPageProg(u08 bufNum, u16 pageAddr, u08 withErase);

//! Main memory page to buffer transfer
/// A page of data can be transferred from the main memory to either
/// buffer 1 or buffer 2.
void dfPageToBufferTransfer(u08 bufNum, u16 pageAddr);

//! Page to buffer Compare
/// A page of data in main memory can be compared to the data in buffer 1
/// or buffer 2 after the transferation of a page to the buffer 1 or buffer 2.
/// This function is specific to use after a dfPageToBufferTransfer function.
/// Do not use this function to compare them after a buffer to page
/// program.
/// When the data in the main memory page matches the data in the buffer
/// the function returns 0.
u08 dfPageToBufferCompare(u08 bufNum, u16 pageAddr);

//! Auto page rewrite through buffer
/// This mode is only needed if multiple bytes within a page or multiple
/// pages of data are modified in a random fashion. This mode is a combination
/// of two operations: Main Memory Page to Buffer Transfer and Buffer to Main
/// Memory Page Program with Built-in Erase. A page of data is first transferred
/// from the main memory to buffer 1 or buffer 2, and then the same data (from
/// buffer 1 or buffer 2) is programmed back into its original page of main
/// memory.
void dfAutoPageRewriteThroughBuffer(u08 buffNum, u16 pageAddr);

//! Page erase
/// The optional Page Erase command can be used to individually erase any
/// page in the main memory array allowing the Buffer to Main Memory Page
/// Program without Built-in Erase command to be utilized at a later time.
void dfPageErase(u16 pageAddr);

//! Block erase
/// A block of eight pages can be erased at one time allowing the Buffer to
/// Main Memory Page Program without Built-in Erase command to be utilized to
/// reduce programming times when writing large amounts of data to the device.
void dfBlockErase(u16 blockAddr);

//! Main memory page program through buffer
/// This operation is a combination of the Buffer Write and Buffer to Main
/// Memory Page Program with Built-in Erase operations. Data is first shifted
/// into buffer 1 or buffer 2 from AVR buffer RAM and then programmed into a 
/// specified page in the main memory.
void dfMainMemoryPageProgramThroughBuffer(u08 buffNum, u16 pageAddr, u16 byteAddr, u08 *myBuffer, u16 numBytes);

#endif
//@}
