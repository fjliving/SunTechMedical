/*
 * led.h
 *
 * Created: 10/13/2016 11:17:08 AM
 *  Author: Fred
 */ 

#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define CLASS_IRQ(name, vector) \
	static void name(void) asm(__STRINGIFY(vector)) \
	__attribute__ ((signal, __INTR_ATTRS))

class CLed{

	public:
	CLed(void);
	void led_init(PORT_t* device_port, uint8_t device_pinbm, uint8_t device_timer);
	void blink(uint8_t freq_hz, uint8_t dc);
	
	void led_On(void);
	void led_Off(void);
	
	private:
	uint8_t initTimer(uint8_t timer_type);
	TC_CLKSEL_t calcDivider(uint16_t& dividor);
	void disableInterrupts(void);

	CLASS_IRQ(Timer0CompareISR, TCC0_CCA_vect); 
	CLASS_IRQ(Timer0OvfISR, TCC0_OVF_vect);
	CLASS_IRQ(Timer1CompareISR, TCC1_CCA_vect);
	CLASS_IRQ(Timer1OvfISR, TCC1_OVF_vect);
	
	uint8_t timer_type; // Stores Type of Timer 0 or 1
	uint8_t freq_hz;	// Timer Frequency 1 - 100 Hz
	uint8_t dc;			// Timer Duty Cycle 0 - 100 %
	
	PORT_t*	 device_port;
	uint8_t	 device_pinbm;
	uint8_t	 device_timer;
	};


#endif /* LED_H_ */