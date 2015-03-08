Minimal UART for the ATmega328p
===============================

## Aim

The code in this repository should enable the USART0 module of an atmega328p chip in receive mode.
Most of the time the main loop performs a 'heartbeat' on a LED connected to PC0.
If the sequence `abcd` is send over serial, followed by a newline or carriage-return, it will
switch from doing the heartbeat on PC0 to PC1. If it receives that sequence again, it will switch back.

## The Transmit Buffer

I want a buffered transmission method. Let's call the buffer `TXBuffer`.
I would like to be able to add characters to the buffer send some (not necessarilly all) of them, and add some more.
Let's define two pointer, `TXHead` and `TXTail`.
`TXTail` will point to the next character to be transmitted, `TXHead` to the next memory location into which a character may be written. e.g.
```
	TXBuffer = "abcd000000"
	            ^   ^
	       TXTail   TXHead
```
After one transmittion and adding two more characters to the buffer.
```
	TXBuffer = ".bcdef...."
	             ^    ^
	        TXTail    TXHead
```
If `TXTail == TXHead` then we may not perform a transmission - there are no more characters left to transmit.
If `(TXTail - TXHead) % TX_BUFFER_SIZE == 1` then we may not add any more characters to the buffer (yes, I recognise that this means there is always one byte of the buffer unused).

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
