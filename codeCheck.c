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

ISR(USART_RX_vect)
{
	// Hold LED on for 2 seconds
	int i;
	char recvByte = UDR0;
	PORTC |= (1<<beatPin);
	for(i=0; i<200; i++){
		_delay_ms(10);
	}
	// If we've been told to 'switch', do so
	if (recvByte == 's'){
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
