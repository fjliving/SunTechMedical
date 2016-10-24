/*
 * potentiometer.h
 *
 * Created: 10/18/2016 9:53:09 PM
 *  Author: Fred
 */ 


#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define CLASS_IRQ(name, vector) \
	static void name(void) asm(__STRINGIFY(vector)) \
	__attribute__ ((signal, __INTR_ATTRS))
	

class CPotentiometer{
	public:
	CPotentiometer(void);
	~CPotentiometer(void);
	
	void potentiometer_init(void);
	uint16_t getValue(void);
	uint8_t getPercentage(void);
	
	private:
	CLASS_IRQ(ConversionCompleteISR, ADCB_CH0_vect);
	uint8_t ReadCalibrationByte(uint8_t index);
	
	static uint16_t value;
};

#endif /* POTENTIOMETER_H_ */