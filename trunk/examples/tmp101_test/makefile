# Makefile for avrproject
# Pascal Stang

########### change this lines according to your project ##################
#put the name of the target mcu here (at90s8515, at90s8535, attiny22, atmega603 etc.)
	MCU = atmega32
#	MCU = atmega323
#	MCU = atmega128

#put the name of the target file here (without extension)
	TRG	= tmp101test

#put your C sourcefiles here 

	SRC = $(AVRLIB)/buffer.c $(AVRLIB)/uart.c $(AVRLIB)/rprintf.c $(AVRLIB)/timer.c $(AVRLIB)/vt100.c $(AVRLIB)/i2c.c $(TRG).c

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
buffer.o	: buffer.c	buffer.h
uart2.o		: uart2.c	uart2.h		global.h
rprintf.o	: rprintf.c	rprintf.h	global.h
i2c.o		: i2c.c		i2c.h		global.h
$(TRG).o	: $(TRG).c	global.h	$(KAIKALIB)/tmp101.h

install:
#	uisp -dprog=dapa -v=3 --erase
#	uisp -dprog=dapa -v=3 --upload if=$(TRG).hex
#	uisp -dserial=/dev/ttyUSB0 -speed=115200 -dprog=dasa2 -v=3 --erase
#	uisp -dserial=/dev/ttyUSB0 -speed=115200 -dprog=dasa2 -v=3 --upload if=$(TRG).hex
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --erase
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --upload if=$(TRG).hex



cleanmem :
#	uisp -dprog=dapa -v=3 --erase
#	uisp -dserial=/dev/ttyUSB0 -speed=115200 -dprog=dasa2 -v=3 --erase
	uisp -dpart=$(MCU) -dserial=/dev/ttyUSB0 -dprog=stk500 -v=3 --erase