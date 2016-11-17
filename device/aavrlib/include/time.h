/*******************************************************************************
 *	time.h
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


/**
 * @file time.h
 * @author Dennis Pinto Rivero
 * @date 23 June 2016
 * @brief Time related definitions and functions
 */

#ifndef __TIME
#define __TIME

#include <stdint.h>

/**
 * @brief Time structure definition
 * 
 * Represents time in microsecond precision. Can only represent positive values.
 * Because of posible API changes, it is not recommended to access the struct 
 * members directly, but use the provided functions and macros.
 */
typedef struct {
    /** microseconds \f$(0, 999)\f$*/
    uint16_t us;
    /** miliseconds \f$(0, 2^{32}-1)\f$*/
    uint32_t ms;
} time_t;

/**
 * @brief Time summation
 * 
 * Function to perform a time summation \f$ r = a+b \f$.
 * Example of usage:
 * @code
 * ...
 * time_t time1, time2, result;
 * ...
 * get_uptime(&time1);
 * get_uptime(&time2);
 * time_add(&time1, &time2, &result);
 * ...
 * @endcode
 * 
 * @param op1 First operand
 * @param op2 Second operand
 * @param result Result of the operation op1+op2
 * @return If the operation causes an overflow, returns -1, otherwise, 0.
 */
int8_t time_add(time_t *op1, time_t *op2, time_t *result);


/**
 * @brief Time substraction
 *
 * Function to perform a time substraction \f$ r = a-b \f$.
 * As time_t can not represent negative values, if \f$b > a\f$, \f$ b \f$ is 
 * considered overflowed, so \f$ r = MAX - b + a\f$.
 * Example of usage:
 * @code
 * ...
 * time_t time1, time2, result;
 * ...
 * get_uptime(&time1);
 * get_uptime(&time2);
 * time_sub(&time2, &time1, &result);
 * ...
 * @endcode
 * 
 * @param op1 First operand
 * @param op2 Second operand
 * @param result Result of the operation op1-op2
 * @return [Still undefined]
 */
int8_t time_sub(time_t *op1, time_t *op2, time_t *result);


#endif /* __TIME */
