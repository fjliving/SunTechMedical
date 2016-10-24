/*
 * ui.cpp
 *
 * Created: 10/23/2016 3:47:01 AM
 *  Author: Fred
 */ 

#include "ui.h"
#include <string.h>

CUI::CUI(){	
}

void CUI::ui_init(CSerial* _serial){
	serial = _serial;
	c42048a_init();
	c42048a_set_contrast(LCD_CONTRAST);
	c42048a_blinkrate_init(LCD_BLINKRATE_1Hz_gc);
}


void CUI::sendText(char* msg){
	/* Copy msg to buffer */
	strcpy (lcd_msg, msg);
	
	/* Send Message to LSD */
	c42048a_set_text((uint8_t*) lcd_msg);
	
	/* Send Message to USB Com Port */
	if(udi_cdc_is_tx_ready()){
		while(*msg)
			udi_cdc_putc(*msg++);
	}
	
	/* Send Message to Serial Device */
	serial->sendString(lcd_msg);
}


void CUI::clearUI(void){
	/* Clear User Interface */
	uint8_t msg[] = "       ";
	c42048a_set_text(msg);
}
