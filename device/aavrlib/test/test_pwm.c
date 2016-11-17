/*******************************************************************************
 *  test_pwm.c
 *
 *  PWM test
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
#include <avr/power.h>
#include <stdint.h>

#include <uc/system.h>
#include <uc/timers.h>
#include <boards/arduinoUNO.h>



#define FREQ_CNT (20)
#define DUTY_HALF (FREQ_CNT/2)



int main( void ) {

	system_init();
	
	IOPORT_CONFIG(OUTPUT, PORT_B, _BV(PIN_5) | _BV(PIN_3));
	IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_3));

	timer0_pcorrect_pwm(prescale0_1024, 255, 20);
	
	while(1);
	
	return 0;
}
