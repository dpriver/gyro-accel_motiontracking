/*******************************************************************************
 *  test_softPWM_short.c
 *
 *  Short pulse software generated PWM test
 *  Send a software based PWN signal to the pin 4 of the arduinoUNO board. The 
 *  pulse width comes determined for the analog value readed from the board's 
 *  a0 pin. Each 300ms, the readed and the assigned values are showed via USART.
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
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#include <uc/system.h>
#include <uc/usart.h>
#include <uc/analog.h>
#include <boards/arduinoUNO.h>
#include <softPWM_short.h>



#define FREQ_CNT (20)
#define DUTY_HALF (FREQ_CNT/2)
#define ADC_MASK (0x0)



int main( void ) {

    uint8_t analog_read = 0;
    uint8_t duty_count = 0;

	system_init();
    usart_init();
    softPWM_s_init();
    adc_init(adc_presc_128, adc_ref_vcc, adc_channel_a0, ADC_MASK);
    
    
    IOPORT_CONFIG(INPUT, PORT_A, _BV(PIN_A0));

    softPWM_s_start();

    softPWM_s_add_signal(PIN_4, &PORT_B, &PORT_B_V, 0, 124);
    
    
    
	while(1) {
        analog_read = adc_single_read();
        duty_count = (124./255) * analog_read;
        
		usart_print("Readed value: ");
		usart_printnumber8(analog_read);
		usart_print("\tAssigned value: ");
        usart_printnumber8(duty_count);
        usart_print("\n");
        
        softPWM_s_set_pulse_width(0, duty_count);
        
        _delay_ms(300);
    }
	
	return 0;
}
