/*******************************************************************************
 *  HCSR04.c
 *
 *  HC-SR04 ultrasonic sensor
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
 *******************************************************************************/

#include <stdint.h>
#include <util/delay.h>

#include "peripherals/HCSR04.h"
#include "systick.h"


void ultrasonic_init() {
        
}


int16_t ultrasonic_measure(uint8_t volatile *trig_port, uint8_t trig_pin, 
        uint8_t volatile *echo_port, uint8_t echo_pin) {
    
    time_t time1, time2, time_interval;
    //uint16_t time;
    time_t timeout;

    //IOPORT_VALUE(HIGH, *trig_port, trig_pin);
    *trig_port |= trig_pin;
    // wait 10us
    _delay_us(10);
    //IOPORT_VALUE(LOW, *trig_port, trig_pin);
    *trig_port &= ~trig_pin;
    // wait for echo to be set (timeout is needed)

    start_timeout(1, &timeout);
    //while ( !IOPORT_READ(*echo_port, echo_pin) ) {
    while(!(*echo_port & echo_pin)) {
        if (timeout_expired(&timeout)) {
            return -1;
        }
    }
    
    get_uptime(&time1);
    
    // wait for echo to be clear (timeout is needed)
    start_timeout(30, &timeout);
    //while (IOPORT_READ(*echo_port, echo_pin)){
    while(*echo_port & echo_pin) {
        if (timeout_expired(&timeout)) {
            return -1;
        }
    }
    
    get_uptime(&time2);
    
    time_sub(&time2, &time1, &time_interval);
    
    
    return ((time_interval.ms << 10) + time_interval.us) >> 6;
    
    
    /*
    if (us_2 > us_1) {
        time = ((ms_2 - ms_1) << 10) + (us_2 - us_1);
    }
    else {
        time = ((ms_2 - ms_1) << 10) + (us_1 - us_2);
    }
    
    // aproximate time/58 with time/64 to optimize calculations
    return time >> 6;
    
    return 0;
    */
}
