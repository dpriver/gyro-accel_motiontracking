/*******************************************************************************
 *	systick.h
 *
 *  system tick
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
 ********************************************************************************/


/**
 * @file systick.h
 * @author Dennis Pinto Rivero
 * @date 2015
 * @brief System tick
 * 
 * System tick is a scheme to manage time. It provides functionality to carry 
 * the uptime, perform time measurements and other time related functionality 
 * like timeouts and delays.
 * 
 * It counts the uptime up to \f$ 2^{32}\f$ milisecons, which are \f$49.7\f$ 
 * days, in microseconds precision.
 */
 
#ifndef __SYSTICK
#define __SYSTICK

#include <stdint.h>
#include "time.h"

#if defined(__DOXYGEN__)
    /**
     * @brief The maximum value returned by the get_micros function
     */
    #define MAX_MICROS_COUNT
#else
    /* (uint32_t)(UINT32_MAX-1 << 10) + 999 */
    #define MAX_MICROS_COUNT ((uint32_t)((UINT32_MAX << 10) + 999))
    //#define MAX_MICROS_COUNT (4294967271)
#endif

/**
 * @brief System tick init
 * 
 * Initialize System tick and begin counting the uptime
 */
void systick_init();


/**
 * @brief Obtain system uptime
 * 
 * Function to obtain the system uptime in a time time_t structure. The uptime 
 * count is started by the systick_init function. When using this function, is 
 * important to note that the system uptime will overflow at \f$ 2^{32}-1\f$ 
 * miliseconds. (49.7 days)
 * 
 * Example of usage:
 * @code
 * ...
 * time_t time;
 * ...
 * get_uptime(&time);
 * ...
 * @endcode
 * 
 * @param time The system uptime.
 */
void get_uptime(time_t *time);


/**
 * @brief Obtain the system uptime in microsecons.
 * 
 * This function returns the system uptime in microseconds. Especial attention 
 * should be paid when using this function; as the return value is in the range 
 * from \f$0\f$ to \f$2^{23}-1\f$ microseconds, which is 1.19 hours, it should 
 * not be used to measure bigger time intervales.
 * 
 * @return The system uptime in microseconds.
 */
uint32_t get_micros();


/**
 * @brief Makes a delay in milliseconds precision.
 * 
 * Function to perform a busy-loop delay in milliseconds precision. Internally, 
 * this function loops reading the uptime. That means that the delay should be 
 * precise enough for most applications requiring a milliseconds delay. Since 
 * the interruptions are not disabled, the delay time may be actually a little 
 * bigger, but not smaller than the requested value.
 * 
 * @warning overflow is still not taken into account, so unexpected behaviour 
 * can ocurr.
 * 
 * @param ms The number of milliseconds to delay.
 */
void delay_ms(uint16_t ms);


/**
 * @brief Timeout start
 * 
 * This function, in conjuntion with timeout_expired, are used to manage 
 * timeouts.
 * 
 * Example of usage:
 * @code
 * ...
 * time_t timeout;
 * ...
 * start_timeout((uint16_t)200, &timeout);
 * do {
 * ...
 * } while (!timeout_expired(&timeout));
 * ...
 * @endcode
 * 
 * @param ms Milliseconds at which a timeout is considered to be reached.
 * @param timeout time_t structure used internally by timeout functions.
 */
void start_timeout(uint16_t ms, time_t *timeout);


/**
 * @brief Time summation
 * 
 * This function, in conjuntion with start_timeout, are used to manage 
 * timeouts.
 * 
 * Example of usage:
 * @code
 * ...
 * time_t timeout;
 * ...
 * start_timeout((uint16_t)200, &timeout);
 * do {
 * ...
 * } while (!timeout_expired(&timeout));
 * ...
 * @endcode
 * 
 * @param timeout time_t structure used internally by timeout functions.
 * @return false if the timeout has not been reached, or true otherwise
 */
uint8_t timeout_expired(time_t *timeout);


#endif /* __SYSTICK */
