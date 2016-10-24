/**
 * miniproject.h
 *
 * Created: 10/23/2016 9:39:23 AM
 * \author Fred
 *
 * \version $Revision: 1.5 $
 *
 * \brief Suntech MiniProject using AVR XMEGAB1
 * 
 * Contact: Fred.Livingston@gmail.com 
 */ 


#ifndef MINIPROJECT_H_
#define MINIPROJECT_H_


#undef VERSION
#define VERSION “Version 1.0”

/**************************************************
* Version Module - SunTech MiniProject
*
* Copyright 2016 Frederick J Livingston
* All Rights Reserved
*
* The information contained herein is confidential
* property of Company. The use, copying, transfer or 
* disclosure of such information is prohibited except
* by express written agreement with Company.
*
* 10/23/2016 - Version 1.0 - ROM ID 78-130
*		Initial Beta release
**/		




#include "potentiometer.h"
#include "led.h"
#include "deviceio.h"
#include "ui.h"
#include "serial.h"


/*! \name GPIO Connections of LEDs
 */
//! @{
#define LED0_GPIO_PORT PORTB
#define LED0_GPIO_PIN  PIN4_bm
#define LED0_TIMER     0   
#define LED1_GPIO_PORT PORTB
#define LED1_GPIO_PIN  PIN5_bm
#define LED1_TIMER     1   


#endif /* MINIPROJECT_H_ */