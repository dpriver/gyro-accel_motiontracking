/*******************************************************************************
 *	arduinoUNO.h
 *
 *  Definitions and macros for the arduinoUNO board
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


// Digital IOport B
#define PIN_0	(PORTD0)
#define PIN_1	(PORTD1)
#define PIN_2	(PORTD2)
#define PIN_3	(PORTD3)
#define PIN_4	(PORTD4)
#define PIN_5	(PORTD5)
#define PIN_6	(PORTD6)
#define PIN_7	(PORTD7)

// Digital IOPort C
#define PIN_8	(PORTB0)
#define PIN_9	(PORTB1)
#define PIN_10	(PORTB2)
#define PIN_11	(PORTB3)
#define PIN_12	(PORTB4)
#define PIN_13	(PORTB5)

// Analog IOPort
#define PIN_A0	(PORTC0)
#define PIN_A1	(PORTC1)
#define PIN_A2	(PORTC2)
#define PIN_A3	(PORTC3)
#define PIN_A4	(PORTC4)
#define PIN_A5	(PORTC5)


// _MODE values
// SET can be used for both _CONFIG_MODE and _VALUE_MODE
#define SET		= (

// _CONFIG_MODE values
#define OUTPUT 	|= (
#define INPUT 	&= ~(

//	_VALUE_MODE values
#define HIGH	|= (
#define LOW		&= ~(

// _PORT values
#define PORT_A	DDRC
#define PORT_B	DDRD
#define PORT_C	DDRB


#define PORT_A_V PORTC
#define PORT_B_V PORTD
#define PORT_C_V PORTB

#define PORT_A_R PINC
#define PORT_B_R PIND
#define PORT_C_R PINB

// Configure and set values for digital I/O pins
// for example:
// 	pins 0, 3, 5 as output    
//	DIGITAL_CONFIG(OUTPUT, PORT_B, _BV(PIN0) | _BV(PIN3) | _BV(PIN5));

// 	pins 8, 12, 13 as input    
//	DIGITAL_CONFIG(OUTPUT, PORT_C, _BV(PIN8) | _BV(PIN12) | _BV(PIN13));

// 	pins 0, 3 as 1, and 11 as 0
//	DIGITAL_VALUE(HIGH, PORT_B, _BV(PIN0) | _BV(PIN3));
//	DIGITAL_VALUE(LOW, PORT_C, _BV(PIN11));

// Of course, unless included in the mask, or invoked in SET mode, the other
// port pins keep their previous value and configuration.

#define IOPORT_CONFIG(_CONFIG_MODE, _PORT,  pinmask) \
	_PORT _CONFIG_MODE pinmask )


#define IOPORT_VALUE(_VALUE_MODE, _PORT, pinmask) \
	_PORT ## _V _VALUE_MODE pinmask )
    
#define IOPORT_READ(_PORT, pinmask) \
	(_PORT ## _R & pinmask)
