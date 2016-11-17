/*******************************************************************************
 *  analog.h
 *
 *  analog comparator and converter
 *
 *
 *  This file is part of aavrlib
 * 
 *  Copyright (C) 2015  Dennis Pinto Rivero
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ********************************************************************************/

// avr defines
#include <avr/io.h>
#include <avr/power.h>
#include <stdint.h>

#include "uc/analog.h"


void adc_init(adc_prescaler_t prescaler, adc_reference_t ref,
		adc_channel_t channel, uint8_t pinmask) {
	power_adc_enable();
    // enable ADC with selected prescaler
    ADCSRA = (1 << ADEN) | (prescaler << ADPS0);
    // select Vref source and analog input pin
    // left adjusted result (8bit resolution)
    ADCSRB = 0x00;
    ADMUX = (ref << REFS0) | (1 << ADLAR) | (channel << MUX0);
    // disable digital input buffer of pins used as analog inputs
    DIDR0 = pinmask;
}

void adc_change_channel(adc_channel_t channel) {
	ADMUX = (ADMUX & ~(0xf)) | (channel << MUX0);
}


uint8_t adc_single_read() {
	ADCSRA &= ~_BV(ADIF);
	
	ADCSRA |= (1 << ADSC);
	
	// wait until the conversion finishes
	while(!(ADCSRA & (1<<ADIF)));
	
	// only reads ADCH because ADC is in left adjusted mode for an 8 bits
	// resoution. is convenient to use ADCH instead of ADCL because ADCH must
	// be always read in order to allow other conversions to start
	return ADCH;
}
