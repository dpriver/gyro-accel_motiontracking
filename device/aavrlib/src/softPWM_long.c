/*******************************************************************************
 *  softPWM_long.h
 *
 *  Short pulse software PWM generation
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

// avr defines
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uc/interrupt.h"
#include "uc/timers.h"
#include "softPWM_long.h"


// NEEDS
/*
 * Change the pwm frequency in runtime
 * Change the duty cycle for each motor indepently
 * 
 */


#define PWM_FREC 200

#define PWM_PRESCALE SOFTPWM_L_PRESC(1024)

#define PWM_TOP_CNT SOFTPWM_L_MAX_WIDTH

/*
 * The idea here is that softPWM.c does not need to worry about the assigned timer.
 * That is timers.h problem.
 * 
 * because all timers should have the same functions with the same function sign,
 * this can be done, and if the timer assigned to softPWM changes in timers.h, 
 * there is no need to make changes in this file.
 * 
 * If the timer mode have to be changed, it should be done in the macros labeled as
 * _EXP2. For example, if we want to work in pwm mode instead of ctc, the following
 * change shoul be made
 * 
 * #define _TIMER_START_EXP2(TIMER) TIMER ## _ctc(PWM_PRESCALE, PWM_TOP_CNT, UINT8_MAX)
 * for
 * #define _TIMER_START_EXP2(TIMER) TIMER ## _fast_pwm(PWM_PRESCALE, PWM_TOP_CNT, UINT8_MAX)
 * 
 * This seems like a pretty cool way to work as long as timers.h keep the current 
 * homogeneous fashion.
 */

// three level recursion needed to work with PWM_TIMER macro defined in timers.h
#define _TIMER_START_EXP2(TIMER) TIMER ## _ctc(PWM_PRESCALE, PWM_TOP_CNT)
#define _TIMER_START_EXP1(TIMER) _TIMER_START_EXP2(TIMER)

#define _TIMER_STOP_EXP2(TIMER)   TIMER ## _stop();
#define _TIMER_STOP_EXP1(TIMER)   _TIMER_STOP_EXP2(TIMER);

#define _TIMER_SET_DUTY_CNT_EXP2(TIMER, count) TIMER ## _ctc_set_interrupt_cnt(count)
#define _TIMER_SET_DUTY_CNT_EXP1(TIMER, count) _TIMER_SET_DUTY_CNT_EXP2(TIMER, count)

#define _TIMER_ENABLE_DUTY_EXP2(TIMER, count) TIMER ## _ctc_extra_interrupt(count)
#define _TIMER_ENABLE_DUTY_EXP1(TIMER, count)_TIMER_ENABLE_DUTY_EXP2(TIMER, count)


// Start the timer configured to use by softpwm
#define SOFTPWM_TIMER_START() _TIMER_START_EXP1(SOFTPWM_L_TIMER)

// Stop the timer configured to use by softpwm
#define SOFTPWM_TIMER_STOP() _TIMER_STOP_EXP1(SOFTPWM_L_TIMER)

// Enable the extra ctc interrupt for duty
#define SOFTPWM_TIMER_ENABLE_DUTY() _TIMER_ENABLE_DUTY_EXP1(SOFTPWM_L_TIMER, UINT8_MAX)

// Set duty count in the timer configured to use by softpwm
#define SOFTPWM_TIMER_SET_DUTY_COUNT(count) _TIMER_SET_DUTY_CNT_EXP1(SOFTPWM_L_TIMER, count)



// variable definitions

// should have a value between 1 and PWM_TOP_CNT
uint8_t duty_count[SOFTPWM_L_MAX_SIGNALS];

// duty count set is made in two steps, so no signal is hanged in the middle of a pulse
uint8_t updated_duty_count[SOFTPWM_L_MAX_SIGNALS];

// The current duty count, This is usefull to compare the signals' duty in DUTY_ISR
uint8_t curr_duty;

// PIN_x, defined in arduinoUNO.h
uint8_t signal_pin[SOFTPWM_L_MAX_SIGNALS];

uint8_t signal_enabled[SOFTPWM_L_MAX_SIGNALS];

// PORT_x defined in arduinoUNO.h
volatile uint8_t *signal_port[SOFTPWM_L_MAX_SIGNALS];

uint8_t curr_signal;




// ctc top
// Set all signals
// set the first compare interrupt value
//SOFTPWM_L_TOP_ISR() {
INTERRUPT(__vector_softpwm_l_top_isr) {
    // min value greater than current count
    int8_t i;
    uint8_t min_value = UINT8_MAX;

    // Set all signals
    for(i = SOFTPWM_L_MAX_SIGNALS-1 ; i >= 0 ; i-- ) {
        
        // update duty counts
        duty_count[i] = updated_duty_count[i];
        
        if ( duty_count[i] > 0) {
            // set signals' pin and look for the next to clear
            //IOPORT_VALUE(HIGH, *(signal_port[i]), signal_pin[i]);
            *(signal_port[i]) |= signal_pin[i];
            if(duty_count[i] < min_value) {
                min_value = duty_count[i];
            }
        }
    }
    
    curr_duty = min_value;
    SOFTPWM_TIMER_SET_DUTY_COUNT(curr_duty);
}

// ctc compare interrupt
// Clear the corresponding signals and set the new compare interrupt value
// each timer_prescaler cycles, an interrupt can potencially be triggered, so this function
// have to be checked for time constraints
//SOFTPWM_L_DUTY_ISR() {
INTERRUPT(__vector_softpwm_l_duty_isr) {
    int8_t i;
    uint8_t min_value = UINT8_MAX;

    for(i = SOFTPWM_L_MAX_SIGNALS-1 ; i >= 0 ; i-- ) {
        if( duty_count[i] == curr_duty ) {
            //IOPORT_VALUE(LOW, *(signal_port[i]), signal_pin[i]);
            *(signal_port[i]) &= ~signal_pin[i];
        }
        if( (duty_count[i] > curr_duty) && (duty_count[i] < min_value)) {
            min_value = duty_count[i];
        }
    }
    
    curr_duty = min_value;
    SOFTPWM_TIMER_SET_DUTY_COUNT(curr_duty);
}


void softPWM_l_init() {
    int8_t i;
    curr_signal = 0;
    
    for(i = SOFTPWM_L_MAX_SIGNALS-1; i>=0 ; i--) {
       duty_count[i] = 0;
       updated_duty_count[i] = 0;
       signal_enabled[i] = 0;
    }
    
    interrupt_attach(SOFTPWM_L_TOP_int, __vector_softpwm_l_top_isr);
    interrupt_attach(SOFTPWM_L_DUTY_int, __vector_softpwm_l_duty_isr);
    
    SOFTPWM_TIMER_START();
    SOFTPWM_TIMER_ENABLE_DUTY();
}


int8_t softPWM_l_add_signal(uint8_t pin, volatile uint8_t *config_port, 
    volatile uint8_t *data_port, uint8_t slot, uint8_t pulse_width) {
        
    if (slot >= SOFTPWM_L_MAX_SIGNALS)
        return -1;

    if (pulse_width > PWM_TOP_CNT)
        return -1;

    if (signal_enabled[slot])
        return -1;

    signal_pin[slot] = _BV(pin);
    signal_port[slot] = data_port;
    signal_enabled[slot] = 1;
    updated_duty_count[slot] = pulse_width;
    
    //IOPORT_CONFIG(OUTPUT, *config_port, pin);
    *config_port |= pin;
    
    return slot;
}


int8_t softPWM_l_remove_signal(uint8_t slot) {
    
    if (slot >= SOFTPWM_L_MAX_SIGNALS)
        return -1;
    
    updated_duty_count[slot] = 0;
    signal_enabled[slot] = 0;
    
    //IOPORT_VALUE(LOW, *(signal_port[slot]), signal_pin[slot]);
    *(signal_port[slot]) &= signal_pin[slot];
    
    return 0;
}


int8_t softPWM_l_set_pulse_width(uint8_t slot, uint8_t pulse_width) {
    if (slot >= SOFTPWM_L_MAX_SIGNALS)
        return -1;
    
    if (pulse_width > PWM_TOP_CNT)
        return -1;

    if (!signal_enabled[slot])
        return -1;
    
    updated_duty_count[slot] = pulse_width;
    
    return pulse_width;
}


void softPWM_l_start() {
    SOFTPWM_TIMER_START();
    SOFTPWM_TIMER_ENABLE_DUTY();
}


void softPWM_l_stop() {
    SOFTPWM_TIMER_STOP();
}
