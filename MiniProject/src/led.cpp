/*
 * led.cpp
 *
 * Created: 10/13/2016 11:16:37 AM
 *  Author: Fred
 */

#include "led.h"


#define CPU_HZ 24000000

extern CLed led0;
extern CLed led1;

CLed::CLed(void){
	freq_hz      = 0;
	dc           = 0;
}

void CLed::led_init(PORT_t* _device_port, uint8_t _device_pinbm, uint8_t _device_timer){
	device_port  = _device_port;
	device_pinbm = _device_pinbm;
	device_timer = _device_timer;
	
	// Timer Clock was disabled initially in sysclk_init()
	// Must re-activate the Timer clock before configuring its registers 
	if(device_timer == 0)
		PR.PRPC &= ~PR_TC0_bm; // Clear Timer 0 bit in Power Reduction PortC Register
	else
		PR.PRPC &= ~PR_TC1_bm; // Clear Timer 1 bit in Power Reduction PortC Register
		
	/* Configure Led Pin for Output */
	device_port->DIR |= device_pinbm;
	
	/* Turn off Led Pin */
	led_Off();
}

void CLed::blink(uint8_t _freq_hz, uint8_t _dc){
	/* Disable Timer interrupt */
	disableInterrupts();
	
	if(_dc > 0 && _dc <= 100 && _freq_hz > 0 && _freq_hz <= 100){
		dc      = _dc;
		freq_hz = _freq_hz;
		initTimer(device_timer);
	}
	else{
		dc      = 0;
		freq_hz = 0;
	}
}

void CLed::disableInterrupts(void){
	/* Make sure LED is off */
	led_Off();
	
	/* Disable Timer interrupt */
	if(timer_type == 0){
		TCC0.INTCTRLA = TC_OVFINTLVL_OFF_gc;
		TCC0.INTCTRLB = TC_OVFINTLVL_OFF_gc;
	}
	else{
		TCC1.INTCTRLA = TC_OVFINTLVL_OFF_gc;
		TCC1.INTCTRLB = TC_OVFINTLVL_OFF_gc;
	}
}


void CLed::led_On(void){
	/* Turn On Led Pin */
	device_port->OUT &=  ~device_pinbm; // Pin goes low
}

void CLed::led_Off(void){
	/* Turn Off Led Pin */
	device_port->OUT |=  device_pinbm; // Pin goes high
}


void CLed::Timer0CompareISR(void){
	led0.led_Off();
}

void CLed::Timer0OvfISR(void){
	led0.led_On();
}

void CLed::Timer1CompareISR(void){
	led1.led_Off();
}

void CLed::Timer1OvfISR(void){
	led1.led_On();
}

TC_CLKSEL_t CLed::calcDivider(uint16_t& dividor){
	uint16_t smallest_div = CPU_HZ / freq_hz / 0xFFFF;
	
	if (smallest_div < 1) {
		dividor    = 1;
		return TC_CLKSEL_DIV1_gc;
		} else if (smallest_div < 2) {
		dividor    = 2;
		return TC_CLKSEL_DIV2_gc;
		} else if (smallest_div < 4) {
		dividor    = 4;
		return TC_CLKSEL_DIV4_gc;
		} else if (smallest_div < 8) {
		dividor    = 8;
		return TC_CLKSEL_DIV8_gc;
		} else if (smallest_div < 64) {
		dividor    = 64;
		return TC_CLKSEL_DIV64_gc;
		} else if (smallest_div < 256) {
		dividor    = 256;
		return TC_CLKSEL_DIV256_gc;
		} else {
		dividor    = 1024;
		return TC_CLKSEL_DIV1024_gc;
	}
}


uint8_t CLed::initTimer(uint8_t _timer_type){
	uint8_t retval = 0x00;
	timer_type = _timer_type;

	/* Calculate the smallest divider for the requested frequency
	   related to the CPU frequency */
	uint16_t dividor = 0;
	
	if(timer_type == 0)
		TCC0.CTRLA = calcDivider(dividor);
    else
		TCC1.CTRLA = calcDivider(dividor);

	/* Calculate the period from the divider */
	if(timer_type == 0)
		TCC0.PER = CPU_HZ / dividor / freq_hz;
	else
		TCC1.PER = CPU_HZ / dividor / freq_hz;
	
	/* Configure the timer for Single Slope PWM counting. */
	if(timer_type == 0)
		TCC0.CTRLB = TC_WGMODE_SS_gc;
	else
		TCC1.CTRLB = TC_WGMODE_SS_gc; 
	
	/* Configure the Duty Cycle */
	if(timer_type == 0)
		TCC0.CCA = dc * TCC0.PER / 100;
	else
		TCC1.CCA = dc * TCC0.PER / 100;
	
	/* Configure timer to generate an interrupt at bottom */ 
	if(timer_type == 0)
		TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;  
	else
		TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc; 
	
	/* Configure timer to generate an interrupt at compare. */
	if(timer_type == 0)
		TCC0.INTCTRLB = TC_OVFINTLVL_LO_gc;  
	else
		TCC1.INTCTRLB = TC_OVFINTLVL_LO_gc;  
	
	/* Make sure our period is at least 100 ticks so we are able to provide
	   a full range (0-100% duty cycle) */
	if ((timer_type == 0 && TCC0.PER < 100) || (timer_type == 1 && TCC1.PER < 100)) {
		retval = 0x01;
	}
		
	return retval;
}






