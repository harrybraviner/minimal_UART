/* Code copyright Harry Braviner March 2015

codeCheck.c

Intention is to make the 328 act as a UART slave, receiving
bytes passively. When a certain sequence is received, it should
light an LED for one second.

*/

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "myUART.c"
#include <util/delay.h>

unsigned char beatPin = 0;	// Which pin do we flash?
unsigned char offPin = 1;	// Set this one to off

char *codeWord = "abcd";	// The codeword we must type to change the LED
int codeLength = 4;
int codeChar = 0;					// Which character are we currently looking for?

ISR(USART_RX_vect)
{
	char recvByte = UDR0;

	// Is the letter consistent with the code?
	if (recvByte == codeWord[codeChar]){
		codeChar++;
	} else {
		codeChar = 0;
	}

	// If we've now received the complete code, switch LEDs
	if (codeChar == codeLength){
		codeChar = 0;
		beatPin = 1 - beatPin;
		offPin = 1 - offPin;
		PORTC &= ~(1<<offPin);
	}
}

int main(){
	// Set PC0 as output
	DDRC |= (1<<DDC0);
	// Set PC1 as output
	DDRC |= (1<<DDC1);

	enableUART();

	// Enable interrupts
	sei();

	PORTC &= ~(1<<offPin);
	// 'Heartbeat'
	for(;;){
		int i;
		PORTC &= ~(1<<offPin);	// Just make sure it's off
		PORTC &= ~(1<<beatPin);
		for(i=0; i<60; i++){
			_delay_ms(10);
		}
		PORTC |= (1<<beatPin);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
		PORTC &= ~(1<<beatPin);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
		PORTC |= (1<<beatPin);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
	}

	return 0;
}
