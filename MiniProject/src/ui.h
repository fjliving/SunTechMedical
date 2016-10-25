/**
 * \file ui.h
 * \brief User Interface Class Object Header
 *
 * Created: 10/23/2016 3:47:17 AM
 * Author: Fred
 */ 

#ifndef UI_H_
#define UI_H_

#include <asf.h>
#include <avr/io.h>
#include "serial.h"

#define LCD_DISPLAY_SIZE   7 //!< The maximum text length visible text string on the LCD
#define LCD_CONTRAST 60		 //!< LCD Contrast

/**
 * \class CUI ui.h "ui.h"
 * \brief User Interface Class
 */ 
class CUI{

	public:
	CUI(void);
	void ui_init(CSerial* serial);
	void sendText(char* msg);
	void clearUI(void);

	private:
	CSerial* serial;
	char    lcd_msg[LCD_DISPLAY_SIZE];
	
};

#endif /* UI_H_ */