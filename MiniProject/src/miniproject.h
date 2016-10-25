/**
 * \file miniproject.h
 * \brief  MiniProject Main Function Header
 *
 * Created: 10/23/2016 9:39:23 AM
 * Author Fred
 */ 

/**
 * \mainpage SunTech Medical Mini Project Documentation
 * \version 1.0 (alpha)
 * \author Fred Livingston (Fred.Livingston@gmail.com)
 *
 * \par Source Code
 *
 * https://github.com/fjliving/SunTechMedical
 *
 * \par Firmware Description
 * -# LED0 will be off when PA0 is unconnected
 * -# LED0 will blink @ 1HZ (50% duty cycle) when PA0 and PA3 are connected
 * -# LED0 will blink @ 2HZ (75% duty cycle) when PA0 and PA1 are connected
 * -# LED1 will be off when PA5 is unconnected
 * -# When PA5 is connected to PA3, LED1 will blink at a rate from 1HZ to 100HZ depending on the setting of the variable resistor located next to the LEDs
 * -# When PA6 is connected to ground, 'SunTech ' will be continually sent out the serial port and via USB. "SunTech " will also appear on the LCD display
 * -# When PA7 is connected to Vcc, 'Medical ' will be continually sent out the serial port and via USB. "SunTech " will also appear on the LCD display
 *
 * \par Virtual COM Settings
 *  BAUDRATE	115200
 *  STOPBITS	1
 *  PARITY      NONE
 *  DATABITS    8
 * 
 * \par README
 *
 * Main function that starts the application (through miniproject.cpp)
 * The main function initialize the following objects:
 * -# CDeviceIO Device I/O Class (through deviceio.h)
 * -# CLed LED Class (through led.h)
 * -# CPotentiometer Potentiometer Class (through potentiometer.h)
 * -# CSerial USART Serial Class (through serial.h)
 * -# CUI User Interface Class (through ui.h)
 * \n \n The Algorithm waits for and Hardware Interrupt and then performs a task based on the the connected pins found in the Firmware Description
 *
 * \par Device Target
 * AVR XMEGAB1 
 * \n XMEGA-B1 Xplained Getting Started Guide http://www.atmel.com/Images/doc8439.pdf
 */

#ifndef MINIPROJECT_H_
#define MINIPROJECT_H_

#undef VERSION
#define VERSION “Version 1.0 (alpha)”

#include "potentiometer.h"
#include "led.h"
#include "deviceio.h"
#include "ui.h"
#include "serial.h"

/*! \name GPIO Connections of LEDs
 */
//! @{
#define LED0_GPIO_PORT PORTB		//!< Led0 Port on XPlained board.
#define LED0_GPIO_PIN  PIN4_bm		//!< Led0 Pin Bit mask on XPlained board.
#define LED0_TIMER     0			//!< Led0 Timer Device
#define LED1_GPIO_PORT PORTB		//!< Led1 Port on XPlained board.
#define LED1_GPIO_PIN  PIN5_bm		//!< Led1 Pin Bit mask on XPlained board.
#define LED1_TIMER     1			//!< Led1 Timer Device

/// Function Prototypes Declaration
void task(void);

#endif /* MINIPROJECT_H_ */