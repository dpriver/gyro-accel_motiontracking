/*******************************************************************************
 *	test_infrared.c
 *
 *  infrared NEC protocol test
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

#include <avr/interrupt.h>
#include <uc/usart.h>
#include <uc/system.h>
#include <boards/arduinoUNO.h>
#include <systick.h>
#include <peripherals/infrared.h>

uint8_t packet_count;
uint8_t packet_address;
uint8_t packet_command;


void ir_callback(uint8_t address, uint8_t command);

int main( void ) {
    
    system_init();
    systick_init();
    usart_init();
    ir_receiver_init(ir_callback, nec_decode);
    
    sei();
    
    IOPORT_CONFIG(OUTPUT, PORT_C, _BV(PIN_13));
    IOPORT_VALUE(LOW, PORT_C, _BV(PIN_13));

    IOPORT_CONFIG(INPUT, PORT_B, _BV(PIN_2));
    
    while(1);
    
    /*
    while(debug_index < 31) {
        delay_ms(500);
    }
    
    usart_print("\nPOS      MILIS      MICROS      TIME         DIFF");
    usart_print("\n==================================================");
    uint8_t i;
    for( i = 0; i < debug_index; i++) {
        usart_print("\n[");
        usart_printnumber8(i);
        usart_print("]: ");
        usart_printnumber32(milis[i]);
        usart_print(" ");
        usart_printnumber32(micros[i]);
        usart_print(" -> ");
        usart_printnumber32(time[i]);
        usart_print("  =  ");
        usart_printnumber32(diff[i]);
        
        usart_print("  [");
        usart_printnumber32(isr_time[i]);
        usart_print("]");
    }
    */
    return 0;
}


void ir_callback(uint8_t address, uint8_t command) {
    usart_print("\nReceived [");
    usart_printnumber8(address);
    usart_print("]: ");
    usart_printnumber8(command);
}
