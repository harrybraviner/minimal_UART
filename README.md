Minimal UART for the ATmega328p
===============================

## Aim

The code in this repository should enable the USART0 module of an atmega328p chip in receive mode.
Most of the time the main loop performs a 'heartbeat' on a LED connected to PC0.
If the sequence `abcd` is send over serial, it will switch from doing the heartbeat on PC0 to PC1.
If it receives that sequence again, it will switch back.

## Talking to the 328 over UART

To send UART signals to the atmega I used an FTDI USB-TTL chip, sourced from [eBay](http://www.ebay.com/itm/FTDI-Basic-Breakout-USB-TTL-6-PIN-3-3-5V-With-Free-USB-Cable-For-Arduino-/320907184616?ssPageName=ADME:L:OC:GB:3160).
Note: FTDI hve been bricking chips via Windows - *caveat emptor!*

The command used to talk to the chip is
```
	screen /dev/ttyUSB0 9600,cs8,-parenb,-cstopb
```
You can exit screen using `ctrl-a` followed by `k`.

## TODO List

* Compile the myUART.c file seperately. i.e. include a header, not the source.
