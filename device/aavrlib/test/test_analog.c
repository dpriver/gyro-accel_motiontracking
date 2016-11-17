/*******************************************************************************
 *	test_analog.c
 *
 *  analog test
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
 ******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include "uc/system.h"
#include "uc/usart.h"
#include "uc/analog.h"
#include "boards/arduinoUNO.h"


#define ADC_MASK (0x0)


int main( void ) {
	uint8_t analog_read = 0;
    
	system_init();
	usart_init();
	adc_init(adc_presc_128, adc_ref_vcc, adc_channel_a0, ADC_MASK);
    
	IOPORT_CONFIG(INPUT, PORT_A, _BV(PIN_A0));
	IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_7));
    

	while(1) {
		IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_7));
		analog_read = adc_single_read();
		usart_print("Readed value: ");
		usart_printnumber8(analog_read);
		usart_print("\n");

		_delay_ms(300);
		IOPORT_VALUE(LOW, PORT_B, _BV(PIN_7));
		_delay_ms(300);
	}
	
	return 0;
}
