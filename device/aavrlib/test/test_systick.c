/*******************************************************************************
 *	test_systick.c
 *
 *  System tick test
 *  Print via USART the system uptime each second while switching the led 4 of
 *  the arduinoUNO board.
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
#include <util/delay.h>
#include <stdint.h>

#include <uc/system.h>
#include <uc/usart.h>
#include <boards/arduinoUNO.h>
#include <systick.h>


int main( void ) {
    
    time_t time, time_2;
    
    uint8_t led_state;
    
    system_init();
    usart_init();
    systick_init();
    
    
    IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_4));
    IOPORT_VALUE(LOW,  PORT_B, _BV(PIN_4));
    
    led_state = 0;
    
    while(1) {
        
        if (led_state) {
            IOPORT_VALUE(LOW, PORT_B, _BV(PIN_4));
            led_state = 0;
        }
        else {
            IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_4));
            led_state = 1;
        }
        
        get_uptime(&time);
        //usart_print("\nuptime => ms: ");
        //usart_printnumber32((uint32_t)time.ms);
        //usart_print("  us: ");
        //usart_printnumber32((uint32_t)time.us);
        
        _delay_ms(1);
        
        get_uptime(&time_2);
        
        if( (time_2.ms - time.ms == 1) || 
            ((time_2.ms - time.ms == 2) && ((time_2.us < 100) && (time.us > 900)))) {
            //usart_print("\ncorrect");
        }
        else {
            usart_print("\nError in ms\n");
            usart_print("\nuptime 1 => ms: ");
            usart_printnumber32((uint32_t)time.ms);
            usart_print("  us: ");
            usart_printnumber32((uint32_t)time.us);
            usart_print("\nuptime 2 => ms: ");
            usart_printnumber32((uint32_t)time_2.ms);
            usart_print("  us: ");
            usart_printnumber32((uint32_t)time_2.us);
        }
    }
    
    return 0;
}
