# About #
Kaika AVR library is a Free Software project whose goal is to provide a C library for use with GCC on Atmel AVR microcontrollers.

Kaika (japanese): open access (in a library); open shelves.


# News #
## 2008/Jan/22 ##
  * Add On-line Documentation link in the Project Home page.

## 2008/Jan/14 ##
  * First release.

# Libraries #
**External Hardware Device drivers**

  * TMP101 (tmp101) - Temperature Sensor and Thermostat, Texas Instruments http://www.ti.com/.
  * AT45xxx (df) - Data Flash Memory, Atmel http://www.atmel.com/.
  * TSL2550D (tsl2550 and tsl2550ext) - Ambient Light Sensor with SMBus Interface and Extensions, TAOS http://www.taosinc.com/.


**General**

  * Data Storage (datastorage) - Permit storage and restorage block of data.


**AVR Built-In Peripheral Drivers**

  * Pulse Counter (pulsecounter) - Counts the number of pulses in a period of time.


# Install #
  1. Install the avrgcc toolchain.
  1. Install the Procyon AVRLib by Pascal Stang http://hubbard.engr.scu.edu/embedded/avr/avrlib/.
  1. Install the Kaika AVR Lib.
  1. Export the KAIKALIB environment variable like AVRLIB variable.
