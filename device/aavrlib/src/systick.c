/*******************************************************************************
 *	systick.c
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
 ******************************************************************************/


// avr defines
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "uc/interrupt.h"
#include "uc/timers.h"
#include "uc/usart.h"
#include "time.h"
#include "systick.h"


/*
 * timer1 is a 16bit timer, so a valid configuration can be ctc mode with
 * presc = 8
 * top = 2000,
 * so frequency results in 16000000/(8*2000) = 1000Hz, with a resolution of
 * 1/2 ms.
 *
 * If systick is configured to work in a 8bit timer, it can be ctc mode with
 * presc = 64
 * top = 250
 * so frequency is 1000Hz again, with a resolution of 4ms.
 */

#if SYSTICK_RESOLUTION == 16

    #define PRESC       8
    #define SYSTICK_TOP_CNT     1998

#elif SYSTICK_RESOLUTION == 8

    #define PRESC       64
    #define SYSTICK_TOP_CNT     248

#endif

#define MAX_MS  UINT32_MAX
#define MAX_US  999


#define _TIMER_START_EXP2(TIMER)    TIMER ## _ctc(SYSTICK_PRESC(PRESC), SYSTICK_TOP_CNT)
#define _TIMER_START_EXP1(TIMER)    _TIMER_START_EXP2(TIMER)


// Start the timer configured to use by system tick
#define SYSTICK_TIMER_START()       _TIMER_START_EXP1(SYSTICK_TIMER)


#if SYSTICK_RESOLUTION == 16
    #define GET_RAW_MICROS()        (SYSTICK_CURR_CNT() >> 1)
#elif SYSTICK_RESOLUTION == 8
    #define GET_RAW_MICROS()        (SYSTICK_CURR_CNT() << 2)
#endif

#if SYSTICK_RESOLUTION == 16
    #define COUNT_TO_MICROS(count)        (count >> 1)
#elif SYSTICK_RESOLUTION == 8
    #define COUNT_TO_MICROS(count)        (count << 2)
#endif


// variables to count the system uptime
//static volatile uint32_t curr_ms;
static volatile uint32_t curr_ms;


INTERRUPT(__vector_systick_isr) {
    ++curr_ms;
}

// Init system tick feature
void systick_init() {
    
	curr_ms = 0;
    interrupt_attach(SYSTICK_int, __vector_systick_isr);

    SYSTICK_TIMER_START();
}


// BUG -> Race condition
void get_uptime(time_t *time) {

    uint32_t ms, aux_ms;
    uint16_t us, aux_us;
    uint8_t extra, aux_extra;

    uint8_t oldSREG;

    oldSREG = SREG;

    cli();

    aux_ms = curr_ms;
    aux_us = SYSTICK_CURR_CNT();
    aux_extra = SYSTICK_PEND();

    sei();

    do {
        ms = aux_ms;
        us = aux_us;
        extra = aux_extra;

        /* Atomic access to systick variables, so it is not interrupted by
         * the systick ISR, which would cause a race condition */
        cli();
        aux_ms = curr_ms;
        aux_us = SYSTICK_CURR_CNT();
        aux_extra = SYSTICK_PEND();
        sei();

    } while ((curr_ms != aux_ms) || (extra != aux_extra) ||
            ((aux_us < us) && (ms == aux_ms) ));


   if (extra) {
       ms++;
   }
    SREG = oldSREG;

    time->ms = ms;
    time->us = COUNT_TO_MICROS(us);
}


uint32_t get_micros() {
    uint32_t ms, aux_ms;
    uint16_t us, aux_us;
    uint8_t extra, aux_extra;

    uint8_t oldSREG;

    oldSREG = SREG;

    aux_ms = curr_ms;
    aux_us = SYSTICK_CURR_CNT();
    aux_extra = SYSTICK_PEND();

    sei();

    do {
        ms = aux_ms;
        us = aux_us;
        extra = aux_extra;

        /* Atomic access to systick variables, so it is not interrupted by
         * the systick ISR, which would cause a race condition */
        cli();
        aux_ms = curr_ms;
        aux_us = SYSTICK_CURR_CNT();
        aux_extra = SYSTICK_PEND();
        sei();

    } while ((curr_ms != aux_ms) || (extra != aux_extra) ||
            ((aux_us < us) && (ms == aux_ms) ));


   if (extra) {
       ms++;
   }
    SREG = oldSREG;

    us = COUNT_TO_MICROS(us);

    return (uint32_t)((ms << 10) + (uint32_t)us);
}

void start_timeout(uint16_t ms, time_t *timeout) {

    timeout->ms = curr_ms + ms;
}


uint8_t timeout_expired(time_t *timeout) {
    return (curr_ms > timeout->ms);
}


/* DEPRECATED */
void delay_ms(uint16_t ms) {
    uint32_t _ms = curr_ms + ms;

    // if _ms overflows, this loop prevents from skiping the delay, waiting for
    // curr_ms to also overflow
    //while (curr_ms > _ms);

    // There is a problem with the previous loop... if something interrupts
    // just before it, time enought to actually overpass "ms", this will
    // keep the system delaying for 2^(32) ms... which is ~50 days...


    // A possible aproach is to disable interrupts,
    // then execute the loop while (curr_ms > _ms);

    // actual delay busy wait
    while (curr_ms < _ms);
}
