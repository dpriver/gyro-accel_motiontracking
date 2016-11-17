/*******************************************************************************
 *	time.c
 *
 *  Time related definitions and functions
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

#include <stdint.h>
#include "time.h"

// the 16 and 32 bit registers used in time_t makes this an ineficient calculation
int8_t time_add(time_t *op1, time_t *op2, time_t *result) {
    
    result->ms = op1->ms + op2->ms;
    result->us = op1->us + op2->us;

    // us overflow
    if ( (result->us < op1->us) || (result->us < op2->us) ) {
        result->ms ++;
    }
        
    // ms overflow
    if ( (result->ms < op1->ms) || (result->ms < op2->ms) ) {
        return -1;
    }
        
    return 0;
}

int8_t time_sub(time_t *op1, time_t *op2, time_t *result) {
    uint8_t overflow;

    if (op2->ms > op1->ms) {
        result->ms = op2->ms - op1->ms;
        overflow = 1;
    }
    else {
        result->ms = op1->ms - op2->ms;
        overflow = 0;
    }
    
    if (op2->us > op1->us) {
        result->us = 1000 + op1->us - op2->us;
        result->ms--;
    }
    else {
        result->us = op1->us - op2->us;
    }
    
    if ( overflow ) {
        return -1;
    }
    
    return 0;
}
