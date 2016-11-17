/*******************************************************************************
 *	softPWM_short.h
 *
 *  Short pulse softwape PWM generation
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


#ifndef __SOFTPWM
#define __SOFTPWM


#include <stdint.h>


// max pwm signals
#define SOFTPWM_S_MAX_SIGNALS (10)


#define SOFTPWM_S_MAX_WIDTH (125)

/**
 * @brief Short pulse software PWM initialization
 */
void softPWM_s_init();


/**
 * @brief Add a new PWM signal
 * 
 * @param pin The pin to output the signal. Obtained from a board header.
 * @param config_port The configuration port of the provided pin. Obtained from 
 *        a board header.
 * @param data_port The data port of the provided pin. Obtained from a board 
 *        header.
 * @param slot The internal slot in which the PWM should be stored
 * @param pulse_width the signal's pulse width. Should be less than 
 *        SOFTPWM_S_MAX_WIDTH.
 * @return Returns -1 if the slot is not valid, already occupied or the pulse 
 *         is greater than SOFTPWM_L_MAX_WIDTH. Otherwise, it returns the 
 *         assigned slot.
 */
int8_t softPWM_s_add_signal(uint8_t pin, volatile uint8_t *config_port, 
    volatile uint8_t *data_port, uint8_t slot, uint8_t pulse_width);


/**
 * @brief Remove a signal
 * 
 * @param slot The slot at which the signal is located
 * @return -1 if the slot is invalid or 1 otherwise
 */
int8_t softPWM_s_stop_signal(uint8_t slot);


/**
 * @brief Modify a signal's pulse width
 * 
 * @param slot The slot at which the signal is located.
 * @param pulse_width The new pulse width for the signal.
 * @return -1 if the slot is invalid, not used or the pulse with is greater than 
 *         SOFTPWM_S_MAX_WIDTH. 0 otherwise.
 */
int8_t softPWM_s_set_pulse_width(uint8_t slot, uint8_t pulse_width);


/**
 * @brief Starts the PWM generation after a explicit stop.
 * 
 * Since the PWM generation is started by softPWM_l_init, this function only 
 * makes sense after a call to softPWM_stop.
 */
void softPWM_s_start();


/**
 * @brief Stops the PWM generation.
 * 
 * To start the PWM generation again, softPWM_l_start must be called.
 */
void softPWM_s_stop();


#endif /* SOFTPWM */
