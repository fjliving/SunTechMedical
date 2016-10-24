/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <avr/delay.h>

#include "miniproject.h"



static bool my_flag_autorize_cdc_transfert = false;
bool pin_changed = false;

void task(void);

CPotentiometer pot;
CLed led0;
CLed led1;
CDeviceIO pA0;
CDeviceIO pA1;
CDeviceIO pA3;
CDeviceIO pA5;
CDeviceIO pA6;
CDeviceIO pA7;
CUI ui;
CSerial serial;

int main(void)
{	
	/* Set System Clock to 24 MHz, and USB Clock to 48 MHz */
	sysclk_init();
	
	/* Enable all interrupt level. */
	irq_initialize_vectors();
	
	/* Initialize USB Stack 
		UDI_CDC_DEFAULT_RATEB			 115200
		UDI_CDC_DEFAULT_STOPBITS         CDC_STOP_BITS_1
		UDI_CDC_DEFAULT_PARITY           CDC_PAR_NONE
		UDI_CDC_DEFAULT_DATABITS         8
	*/
	udc_start();
	
	/* Initialize the LCD glass control */
	ui.ui_init(&serial);
	
	/* Initialize the Potentiometer */
	pot.potentiometer_init();
	
	/* Initialize the LEDs */
	led0.led_init(&LED0_GPIO_PORT, LED0_GPIO_PIN, LED0_TIMER);	
	led1.led_init(&LED1_GPIO_PORT, LED1_GPIO_PIN, LED1_TIMER);
	
	/* Initialize Input Pins 
	 * Configure PA0 as input, triggered on both edges.
	 * Configure PA1 as input, triggered on both edges.
	 * Configure PA3 as input, triggered on both edges.
	 * Configure PA5 as input, triggered on both edges. 
	 * Configure PA6 as input, triggered on both edges. 
	 * Configure PA7 as input, triggered on both edges. 
	*/	
	pA0.device_init(PIN0_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_WIREDANDPULL_gc);
	pA1.device_init(PIN1_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_PULLDOWN_gc);
	pA3.device_init(PIN3_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_PULLDOWN_gc);  
	pA5.device_init(PIN5_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_WIREDANDPULL_gc);
	pA6.device_init(PIN6_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_PULLUP_gc);
	pA7.device_init(PIN7_bp, PORT_ISC_BOTHEDGES_gc, PORT_OPC_PULLDOWN_gc);
			
	/* Enable Interrupts */
	cpu_irq_enable(); // sei();
	
	while (1)
	{
		task();
	}
}


void task(void)
{
	if(pin_changed){
		
		/* Reset Interrupt Flag */
		pin_changed = false;
		
		/* LED0 will blink @ 1HZ (50% duty cycle) when PA0 and PA3 are connected */
		if(pA0.isConnected() && pA3.isConnected() && !pA5.isConnected()){
			led0.blink(1, 50);
		}	
		/* LED0 will blink @ 2HZ when (75% on duty cycle) when PA0 and PA1 are connected */ 
		else if(pA0.isConnected() && pA1.isConnected()){
			led0.blink(2, 75);
		}
		/* LED0 will be off when PA0 is unconnected */
		else
			led0.blink(0, 0);
		
		
	
		/* When PA5 is connected to PA3, LED1 will blink at a rate from 1HZ to 100HZ depending on the
		    setting of the variable resistor located next to the LEDs */
		if(pA5.isConnected() && pA3.isConnected()){
			led1.blink(pot.getPercentage(), 50);
		}
		/* LED1 will be off when PA5 is unconnected */
		else
			led1.blink(0, 0);
			
		
		/* When PA6 is connected to ground, “SunTech ” will be continually sent out the serial port */ 
		if(!pA6.isHigh()){
			ui.sendText("SunTech ");
			pin_changed = true;
		}
		/* When PA7 is connected to Vcc, “Medical ” will be continually sent out the serial port */ 
		else if(pA7.isHigh()){
			ui.sendText("Medical ");
			pin_changed = true;
		}
		else{ 
			ui.clearUI();
		}
	}
	
	_delay_ms(200);
	
	
}


bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}

void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}

