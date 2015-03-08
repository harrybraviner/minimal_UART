#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Enable USART0 in asynchronous mode, 9600 baud, 8n1 frame format
void enableUART(){
	// Save SREG and clear the global interrupt flag
	unsigned char sreg = SREG;
	cli();

	// Disable the power-reduction bit for USART0
	PRR &= ~(1<<PRUSART0);

	// Set normal speed (U2X0 = 0) and disable multi-processor mode
	UCSR0A &= ~((1<<U2X0)|(1<<MPCM0));
	// Set Baud rate to 9600 (UBRR0 = 103)
	UBRR0H = (unsigned char)(103>>8);
	UBRR0L = (unsigned char)(103);
	UCSR0B |= (1<<RXCIE0);	// Enable RX Complete Interrupt
	UCSR0B &= ~((1<<TXCIE0)|(1<<UDRIE0));	// Disable TX and empty register interrupts
	UCSR0B |= (1<<RXEN0);		// Enable RX
	UCSR0B &= ~(1<<TXEN0);	// Disable TX
	UCSR0B &= ~(1<<UCSZ02);							// 8-bit character size
	UCSR0C |= ((1<<UCSZ00)|(1<<UCSZ01));	// 8-bit character size
	UCSR0C &= ~((1<<UMSEL00)|(1<<UMSEL01));	// Asynchronous USART
	UCSR0C &= ~((1<<UPM00)|(1<<UPM01));			// No parity bit
	UCSR0C &= ~(1<<USBS0);									// Single stop bit

	// Restore the interrupt register
	SREG = sreg;
}
