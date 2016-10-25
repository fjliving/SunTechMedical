/**
 * \file potentiometer.cpp
 * \brief Potentiometer Class Object Source
 * Created: 10/13/2016 5:29:54 PM
 * Author: Fred
 */ 

#include "potentiometer.h"
#include <avr/pgmspace.h>
#include <stddef.h>

uint16_t CPotentiometer::value;

/**
 * \brief Default Constructor
 */
CPotentiometer::CPotentiometer(void){
	value = 0;
}

/**
 * \brief Default DeConstructor
 */
CPotentiometer::~CPotentiometer(void){	
}

/**
 * \brief Reads the production time ADC Calibration values from memory
 * \param index	ROM Address Index      
 * \return the ADC Calibration values
 */
uint8_t CPotentiometer::ReadCalibrationByte(uint8_t index){
		uint8_t result;
		NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
		result = pgm_read_byte(index);
		
		NVM_CMD = NVM_CMD_NO_OPERATION_gc;
		return(result);
}

/**
 * \brief Initialize the Potentiometer
 */
void CPotentiometer::potentiometer_init(void){
	
	// ADC Clock was disabled initially in sysclk_init()
	// Must re-activate the ADC clock before configuring its registers (we're using ADCB)
	PR.PRPB &= ~PR_ADC_bm; // Clear ADC bit in Power Reduction Port B Register
	
	// Calibration values are stored at production time
	// Load stored bytes into the calibration registers
	// First NVM read is junk and must be thrown away
	ADCB.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL0) );
	ADCB.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL1) );
	ADCB.CALL = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL0) );
	ADCB.CALH = ReadCalibrationByte( offsetof(NVM_PROD_SIGNATURES_t, ADCBCAL1) );
		
	// Place ADC channel in single-ended mode
	// Gain set to 1
	ADCB.CH0.CTRL = ADC_CH_INPUTMODE0_bm;
	
	// Connect potentiometer (PB1) to positive input
	// MUXNEG bits are ignored in single-ended mode
	ADCB.CH0.MUXCTRL = ADC_CH_MUXINT0_bm;

	// Apply no limit to ADC sample rate
	// Put ADC in signed mode
	// Enable Free-run mode (single conversion upon trigger)
	// Resolution set to 12-bit, right justified (11-bit effective in signed mode)
	ADCB.CTRLB = ADC_CONMODE_bm | ADC_FREERUN_bm;
		
	// The ADC runs off of the CPU_per clock
	// The sys_clk_init() sets the CPU_per to 24 MHz
	// Set ADC clock to 188kHz:  CPU_per/128    =>    24MHz/128 = 188kHz
	ADCB.PRESCALER = ADC_PRESCALER2_bm | ADC_PRESCALER0_bm ;

	// Set Vref to Vcc/1.6.  This gives 3.3/1.6 = approx 2.06V
	// With effectively 11-bit resolution, this means each LSB
	// will represent approximately 1 mV.
	ADCB.REFCTRL = ADC_REFSEL0_bm;
	
	// Not implementing Event System so ensure EVCTRL is reading zeros
	ADCB.EVCTRL = 0x00;
	
	// Ensure the ADC complete flag is cleared (by writing a '1' to it)
	ADCB.INTFLAGS = ADC_CH0IF_bm;

	// Configure interrupt on conversion complete with high priority
	ADCB.CH0.INTCTRL = ADC_CH_INTLVL1_bm | ADC_CH_INTLVL0_bm; 

	// Enable ADC, module B
	ADCB.CTRLA = ADC_ENABLE_bm; 
}

/**
 * \brief Get the 12bit ADC Value        
 * \return raw ADC value
 */
uint16_t CPotentiometer::getValue(void){
	return value;
}

/**
 * \brief Get the 12bit ADC Value        
 * \return ADC value as a percentange (0 - 100)
 */
uint8_t CPotentiometer::getPercentage(void){
	return (value*100)/0xFFF; 
}

/**
 * \brief Interrupt Service Routine for ADC Conversion
 */
void CPotentiometer::ConversionCompleteISR(void){
	value = ADCB.CH0.RES;
}

