# Makefile for avrproject
# Pascal Stang

########### change this lines according to your project ##################
#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
	MCU = atmega32
#	MCU = atmega323
#	MCU = atmega128

#put the name of the target file here (without extension)
	TRG	= tsl2550test

#put your C sourcefiles here 

	SRC = $(AVRLIB)/buffer.c $(AVRLIB)/uart.c $(AVRLIB)/rprintf.c $(AVRLIB)/timer.c $(AVRLIB)/vt100.c $(AVRLIB)/i2c.c $(KAIKALIB)/tsl2550.c $(KAIKALIB)/tsl2550ext.c $(TRG).c

#put additional assembler source file here
	ASRC =

#additional libraries and object files to link
	LIB	=

#additional includes to compile
	INC	= 

#assembler flags
	ASFLAGS = -Wa, -gstabs

#compiler flags
	CPFLAGS	= -g  -Os -Wall -Wstrict-prototypes -I$(AVRLIB) -I$(KAIKALIB) -Wa,-ahlms=$(<:.c=.lst)

#linker flags
	LDFLAGS = -Wl,-Map=$(TRG).map,--cref


########### you should not need to change the following line #############
include $(AVRLIB)/make/avrproj_make

###### dependecies, add any dependencies you need here ###################
buffer.o		: buffer.c		buffer.h
uart2.o		: uart2.c		uart2.h		global.h
rprintf.o	: rprintf.c		rprintf.h
i2c.o			: i2c.c		i2c.h
tsl2550.o		: tsl2550.c	tsl2550.h
tsl2550ext.o		: tsl2550ext.c	tsl2550ext.h
$(TRG).o		: $(TRG).c					global.h

install:
#	uisp -dprog=dapa -v=3 --erase
#	uisp -dprog=dapa -v=3 --upload if=$(TRG).hex
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --erase
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --upload if=$(TRG).hex


cleanmem :
#	uisp -dprog=dapa -v=3 --erase
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --erase