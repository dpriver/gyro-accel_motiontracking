/*******************************************************************************
 *  test_spi_burst.c
 *
 *  spi test in burst mode
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
#include <util/atomic.h>
#include <util/delay.h>

#include <uc/spi.h>


uint8_t caracteres[10] = {0,1,2,3,4,5,6,7,8,9};


void finish(){

	while(1) {
		PORTB |= _BV(PORTB0);
		_delay_ms(250);
		PORTB &= ~_BV(PORTB0);
		_delay_ms(250);
	}
}

int main(void)
{
	sei();
	DDRB = _BV(DDB0);


	PORTB |= _BV(PORTB0);
	spi_send_burst(caracteres, 10, finish);

	while(1);

	return 0;
}
