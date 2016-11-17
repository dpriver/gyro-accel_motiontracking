/*******************************************************************************
 *  HCSR04.h
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

#ifndef __HC_SR04
#define __HC_SR04

#include <stdint.h>


void ultrasonic_init();


int16_t ultrasonic_measure(uint8_t volatile *trig_port, uint8_t trig_pin, 
        uint8_t volatile *echo_port, uint8_t echo_pin);



#endif /* __HC_SR04 */
