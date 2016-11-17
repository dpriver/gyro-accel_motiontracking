/*******************************************************************************
 *	test_HC-SR04.h
 *
 *  HC-SR04 ultrasonic sensor test
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

#include <uc/system.h>
#include <uc/usart.h>
#include <boards/arduinoUNO.h>
#include <peripherals/HCSR04.h>
#include <systick.h>
 
 
 
#define ULTRASONIC_TRIGGER_PIN  PIN_6
#define ULTRASONIC_ECHO_PIN     PIN_7


int main( void ) {

    int16_t distance;

    //system_init();
    systick_init();
    usart_init();
    ultrasonic_init();
    sei();

    IOPORT_CONFIG(OUTPUT, PORT_C, _BV(PIN_13));
    IOPORT_CONFIG(OUTPUT, PORT_B, _BV(ULTRASONIC_TRIGGER_PIN));
    IOPORT_CONFIG(INPUT, PORT_B, _BV(ULTRASONIC_ECHO_PIN));


    usart_print("\nStarted\n===============================\n");


    while(1) {
        distance = ultrasonic_measure(&PORT_B_V, _BV(ULTRASONIC_TRIGGER_PIN), &PORT_B_R, _BV(ULTRASONIC_ECHO_PIN));
     
        if (distance >= 0) {
            usart_print("\nDistance: ");
            usart_printnumber32(distance);
            usart_print(" cm");
        }
        else {
            usart_print("\ntimedout... !!");
        }
        
        delay_ms(1000);
    }

    return 0;
}
