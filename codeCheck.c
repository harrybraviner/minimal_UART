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

ISR(USART_RX_vect)
{
	// Hold LED on for 2 seconds
	int i;
	char recvByte = UDR0;
	PORTC |= (1<<PORTC0);
	for(i=0; i<200; i++){
		_delay_ms(10);
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

	PORTC &= ~(1<<PORTC1);
	// 'Heartbeat'
	for(;;){
		int i;
		if(!(UCSR0A & (1<<RXC0))){
			PORTC &= ~(1<<PORTC0);
		}
		for(i=0; i<60; i++){
			_delay_ms(10);
		}
		PORTC |= (1<<PORTC0);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
		PORTC &= ~(1<<PORTC0);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
		PORTC |= (1<<PORTC0);
		for(i=0; i<10; i++){
			_delay_ms(10);
		}
	}

	return 0;
}
