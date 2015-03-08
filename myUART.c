#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#define TX_BUFFER_SIZE 10

char *TXBuffer;
uint8_t TXTail, TXHead;

uint8_t RXTXEN = 0;	// Bit 0 is "is RX enabled?" FIXME -implement a buffer for RX in here
										// Bit 1 is "is TX enabled?"

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
	if (RXTXEN & 0x1){
		UCSR0B |=  (1<<RXEN0);	// Enable RX
		UCSR0B |=  (1<<RXCIE0);	// Enable RX Complete Interrupt
	} else {
		UCSR0B &= ~(1<<RXEN0);	// Disable RX
		UCSR0B &= ~(1<<RXCIE0);	// Disable RX Complete Interrupt
	}
	UCSR0B &= ~(1<<TXCIE0);		// We won't use the transmission complete interrupt
	if (RXTXEN & 0x2){
		UCSR0B |=  (1<<TXEN0);	// Enable TX
		UCSR0B |=  (1<<UDRIE0);	// Enable Data Register Empty interrupt
	} else {
		UCSR0B &= ~(1<<TXEN0);	// Disable TX
		UCSR0B &= ~(1<<UDRIE0);	// Disable Data Register Empty interrupt
	}
	UCSR0B &= ~(1<<UCSZ02);							// 8-bit character size
	UCSR0C |= ((1<<UCSZ00)|(1<<UCSZ01));	// 8-bit character size
	UCSR0C &= ~((1<<UMSEL00)|(1<<UMSEL01));	// Asynchronous USART
	UCSR0C &= ~((1<<UPM00)|(1<<UPM01));			// No parity bit
	UCSR0C &= ~(1<<USBS0);									// Single stop bit

	// Restore the interrupt register
	SREG = sreg;
}

int enableUARTTX(){
	if (RXTXEN & 0x2)	return 0;	// It's already enabled

	// Allocate memory for the transmit buffer
	TXBuffer = malloc(TX_BUFFER_SIZE*sizeof(char));

	// Set the tail and head to point at the start of the transmit buffer
	TXTail = TXHead = 0;

	if (TXBuffer == NULL) { return -1; }
	else {
		RXTXEN |= 0x2;	// Note that we've enabled it
		enableUART();		// Set all the relevant flags
		return 0;
	}
}

void disableUARTTX(){
	if(!(RXTXEN & 0x2)){ return; // It's already disabled
	} else {
		enableUART();		// Sets the relevant flags
		free(TXBuffer);	// Frees memory from the transmit buffer
		RXTXEN &= ~0x2;	// Note that we've disabled it
	}
}

void enableUARTRX(){
	if (RXTXEN & 0x1) {return;	// It's already enabled
	} else {
		RXTXEN |= 1;
		enableUART();
	}
}

int UARTtransmit(char *TXString, uint8_t N){
	if (!(RXTXEN & 0x2)) return -2;	// Cannot transmit, transmission is not enabled

	if (TXTail <= TXHead) {
		if ((TX_BUFFER_SIZE - (TXHead - TXTail) - 1) < N){
			return -1;	// Cannot transmit, there isn't space in the buffer
		}
	}
	if (TXTail > TXHead) {
		if ((TXTail - TXHead - 1) < N){
			return -1;	// Cannot transmit, there isn't space in the buffer
		}
	}

	// We can transmit, hurray
	int i=0;
	for(i=0; i<N; i++){
		TXBuffer[TXHead] = TXString[i];
		TXHead = (TXHead+1) % TX_BUFFER_SIZE;
	}

	// Enable the Data Register Interrupts
	UCSR0B |= (1<<UDRIE0);

	return 0;
}

int UARTtransmitString(char *TXString){
	return UARTtransmit(TXString, strlen(TXString));
}

ISR(USART_UDRE_vect){
	// Do we have anything to transmit?
	if (TXTail != TXHead){
		UDR0 = TXBuffer[TXTail];
		TXTail = (TXTail+1) % TX_BUFFER_SIZE;
	}
	if (TXTail == TXHead){
		// We have nothing to transmit - disable the interrupt
		UCSR0B &= ~(1<<UDRIE0);
	}
}
