/**
 * \file deviceio.h
 * \brief Contains Device I/O Object Header
 *
 * Created: 10/19/2016 3:00:30 AM
 * Author: Fred
 */ 


#ifndef DEVICEIO_H_
#define DEVICEIO_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define CLASS_IRQ(name, vector) \
	static void name(void) asm(__STRINGIFY(vector)) \
	__attribute__ ((signal, __INTR_ATTRS))

/**
 * \class CDeviceIO
 * \brief Device I/O Class
 */ 
class CDeviceIO{
	public:
	CDeviceIO(void);
	~CDeviceIO(void);
	void device_init(uint8_t device_pin, PORT_ISC_t device_sense, PORT_OPC_t device_conf);
	bool isConnected(void);
	
	private:
	CLASS_IRQ(PinChangedISR, PORTA_INT0_vect); 
	uint8_t device_pin;
	uint8_t device_pin_bm;
	};

#endif /* CDEVICEIO_H_ */