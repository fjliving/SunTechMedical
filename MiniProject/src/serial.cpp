/**
 * \file serial.cpp
 * \brief USART Serial Device Object Source
 * Created: 10/23/2016 12:43:08 PM
 * Author: Fred
 */ 

#include "serial.h"

/**
 * \brief Default Constructor
 */
CSerial::CSerial(void){
}

/**
 * \brief Default DeConstructor
 */
CSerial::~CSerial(void){
}

/**
 * \brief Initialize the Serial Device
 */
void CSerial::serial_init(void){
	
	//Enable receive and transmit
	USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm;
	
	// Baud rate selection
	// BSEL = (24000000 / (2^0 * 16*9600) -1 = 155.25 -> BSCALE = 0
	// FBAUD = ( (24000000)/(2^0*16(155+1)) = 9615.384 -> it's alright	
	USARTC0_BAUDCTRLB = 0;		//Just to be sure that BSCALE is 0
	USARTC0_BAUDCTRLA = 0x9B;	// 155
	
	//Disable interrupts, just for safety
	USARTC0_CTRLA = 0;
	
	//8 data bits, no parity and 1 stop bit
	USARTC0_CTRLC = USART_CHSIZE_8BIT_gc;
	
	//Enable receive and transmit
	USARTC0_CTRLB = USART_TXEN_bm | USART_RXEN_bm; // And enable high speed mode
	
	//TX pin as output
	PORTC_OUTSET = PIN3_bm;
	PORTC_DIRSET = PIN3_bm; 
	
	//RX pin as input
	PORTC_OUTCLR = PIN2_bm;
	PORTC_DIRCLR = PIN2_bm;
}

/**
 * \brief Send one char to Serial Device
 * \param c	character to send        
 */
void CSerial::sendChar(char c){
	//Wait until DATA buffer is empty
	while( !(USARTC0_STATUS & USART_DREIF_bm) ); 
	
	USARTC0_DATA = c;
}

/**
 * \brief Send string to Serial Device
 * \param text	character string to send        
 */
void CSerial::sendString(char *text){
	while(*text)
		sendChar(*text++);
}