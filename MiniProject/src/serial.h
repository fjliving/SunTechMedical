/**
 * \file serial.h
 * \brief USART Serial Device Object Header
 *
 * Created: 10/23/2016 12:43:25 PM
 *  Author: Fred
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>

/**
 * \class CSerial
 * \brief USART Serial Class
 */ 
class CSerial{
	public:
	CSerial(void);
	~CSerial(void);
	
	void serial_init(void);
	void sendChar(char c);
	void sendString(char *text);
	};

#endif /* SERIAL_H_ */