/*******************************************************************************
 *  test_softPWMlong.h
 *
 *  long pulse software generated PWM test
 *  Send a software based PWN signal to the pin 4 of the arduinoUNO board. The 
 *  pulse width comes determined for the analog value readed from the board's 
 *  a0 pin. The readed and the assigned values are showed via USART
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

#include <uc/system.h>
#include <uc/analog.h>
#include <uc/usart.h>
#include <boards/arduinoUNO.h>
#include <softPWM_long.h>


/*
 * SYSTEM DESCRIPTION
 * 
 * Direction PINS =>
 *      PORT_B -> PIN_2
 *      PORT_B -> PIN_3
 * 
 * PWM PINS =>
 *      PORT_B -> PIN_4
 *      PORT_B -> PIN_5
 *
 * Speed set input =>
 *      PORT_A -> 0
 * 
 */


int main( void ) {
    
    uint8_t analog_read, pulse_width;
    
    system_init(),
    usart_init();
    softPWM_l_init();
    adc_init(adc_presc_128, adc_ref_vcc ,adc_channel_a0, 0);
    
    sei();    
    
    // config digital outputs to set direction
    IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_2));
    IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_3));
    
    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_2));
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_3));
    
    // start pwm generation
    softPWM_l_start();
    
    
    // long pulse PWM in pin 4, with duty at 50%
    if (softPWM_l_add_signal(PIN_4, &PORT_B, &PORT_B_V, 0, 15) == -1) {
        usart_print("\n\nError, could not add PWM signal\n");
        return 0;
    }
    
    
    while(1) {
        
        // get speed input
        analog_read = adc_single_read();
        pulse_width = (77./255) * analog_read;
        
        
        usart_print("\nAnalog read: ");
        usart_printnumber8(analog_read);
        
        usart_print("\t\tpulse width: ");
        usart_printnumber8(pulse_width);
       

        if (softPWM_l_set_pulse_width(0, pulse_width) == -1) {
            usart_print("\n\nError, could not set the signal's pulse\n");
            return 0;
        }
    }
    
    return 0;
}
