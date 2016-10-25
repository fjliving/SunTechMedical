/**
 * \file deviceio.cpp
 * \brief Contains Device I/O Object Class
 *
 * Created: 10/19/2016 3:00:09 AM
 * Author: Fred
 */ 

#include "deviceio.h"

extern bool pin_changed;

/**
 * \brief Default Constructor
 */
CDeviceIO::CDeviceIO(void){	
}

/**
 * \brief Default DeConstructor
 */
CDeviceIO::~CDeviceIO(void){
}

/**
 * \brief Initialize the I/O Device
 * \param _device_pin	I/O Pin address          
 * \param device_sense  Interrupt edge type
 * \param device_conf  I/O Configuration
 */
void CDeviceIO::device_init(uint8_t _device_pin, PORT_ISC_t device_sense, PORT_OPC_t device_conf){
	
	device_pin = _device_pin;
	
	switch(device_pin){
		case PIN0_bp:
		device_pin_bm  = PIN0_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN0_bm;						/* set the pin direction to input */
		PORTA.INT0MASK |= PIN0_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN0CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN1_bp:
		device_pin_bm  = PIN1_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN1_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN1_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN1CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN2_bp:
		device_pin_bm  = PIN2_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN2_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN2_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN2CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN3_bp:
		device_pin_bm  = PIN3_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN3_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN3_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN3CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN4_bp:
		device_pin_bm  = PIN4_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN4_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN4_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN4CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN5_bp:
		device_pin_bm  = PIN5_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN5_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN5_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN5CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN6_bp:
		device_pin_bm  = PIN6_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN6_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN6_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN6CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
		
		case PIN7_bp:
		device_pin_bm  = PIN7_bm;						/* store the pin bitmap */
		PORTA.DIR	   &= ~PIN7_bm;						/* set the port direction to input */
		PORTA.INT0MASK |= PIN7_bm;						/* select which pins uses for interrupts 0 */
		PORTA.PIN7CTRL = device_sense | device_conf;	/* pin configuration p.138 */
		break;
	}
	
	/* set the interrupt level */
	PORTA.INTCTRL |= PORT_INT0LVL_HI_gc;	
}

/**
 * \brief Determine if I/O Pin is connected
 * \return true if the the I/O pin is connected to jumper or VCC
 * \return false if the I/O pin is floating or unconnected
 */
bool CDeviceIO::isConnected(void){
	bool retval;
	
	if((PORTA.IN & device_pin_bm))    // if connected to VCC
	retval = true;
	else
	retval = false;
	
	return retval;
}

/**
 * \brief Interrupt Service routine for I/O Device
 */
void CDeviceIO::PinChangedISR(void){
	 pin_changed = true;
}

	
