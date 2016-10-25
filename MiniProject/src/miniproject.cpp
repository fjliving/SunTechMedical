/**
 * \file miniproject.cpp
 * \brief MiniProject Main Function Source
 */

#include <asf.h>
#include <avr/io.h>
#include <avr/delay.h>

#include "miniproject.h"

static bool my_flag_autorize_cdc_transfert = false;
bool pin_changed = false;


CPotentiometer pot; //!< Potentiometer Object
CLed led0;			//!< LED0 Object
CLed led1;			//!< LED1 Object
CDeviceIO pA0;		//!< PORTA PIN0 Object
CDeviceIO pA1;		//!< PORTA PIN1 Object
CDeviceIO pA3;		//!< PORTA PIN3 Object
CDeviceIO pA5;		//!< PORTA PIN5 Object
CDeviceIO pA6;		//!< PORTA PIN6 Object
CDeviceIO pA7;		//!< PORTA PIN7 Object
CUI ui;				//!< User Interface Object
CSerial serial;		//!< Serial Object


/**
 * \brief Miniproject Main Function  
 * \return 0 Always
 */
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
	return 0;
}

/**
 * \brief Main Algorithm executed after an I/O Interrupt
 */
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
		if(!pA6.isConnected()){
			ui.sendText("SunTech ");
			pin_changed = true;
		}
		/* When PA7 is connected to Vcc, “Medical ” will be continually sent out the serial port */ 
		else if(pA7.isConnected()){
			ui.sendText("Medical ");
			pin_changed = true;
		}
		else{ 
			ui.clearUI();
		}
	}
	_delay_ms(200);
}

/**
* \brief USB Interrupt Callback when Enabled
* \return true Always
*/
bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}

/**
* \brief USB Interrupt Callback when Disable
*/
void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}

