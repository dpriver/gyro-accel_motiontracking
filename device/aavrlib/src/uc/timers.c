/*******************************************************************************
 *	timer.h
 *
 *  timers
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


/************************** REGISTERS ******************************************
 * TCCRxA   COM0A1	COM0A0	COM0B1	COM0B0	-		-		WGM01	WGM00
 * TCCRxB	FOC01	FOC0B	-		-		WGM02	CS02	CS01	CS00
 * TCNTx[7:0]
 * OCRxA[7:0]
 * OCRxB[7:0
 * TIMSKx	-		-		-		-		-		OCIE0B	OCIEOA	TOIE0
 * TIFRx	-		-		-		-		-		OCF0B	OCF0A	TOV0
 */

/***************************** PINS ********************************************
 * OCxA
 * OCxB
 */
/*
 * avr AtMega328p have 3 timers (all of them with PWM):
 *	- 8bit compare reg. timer
 *	- 16bit compare reg. timer
 *	- 8bit compare reg. timer with async. operation
 *
 */


/*
 * In programs using timers, timers_init() have to be called at the beggining
 * 
 * Then, if a setup functions is called, the corresponding timer is configured,
 * when a start function is called, the timer is blocked and start counting.
 * anytime, the corresponding stop function can be called to stop and release
 * the timer
 */


/* TODO
 * - Implement software PWMs in timer2
 * - Allow usage of timer1 as system cycle counter for debug pourposes
 */

// avr defines
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> 

#include "uc/timers.h"



void timer0_ctc(prescale0_t prescale, uint8_t top_cnt) {
	power_timer0_enable();
	TCNT0 	= 0;
    
	OCR0A 	= top_cnt;
	TIMSK0 	= _BV(OCIE0A);
    
	TCCR0A 	= (0x2 << WGM20);
	TCCR0B 	= (prescale << CS20);
}


void timer0_ctc_extra_interrupt(uint8_t interrupt_cnt) {
    OCR0B = interrupt_cnt;
    TIMSK0 |= _BV(OCIE0B);
}


void timer0_ctc_set_interrupt_cnt(uint8_t interrupt_cnt) {
    OCR0B = interrupt_cnt;
}


// generate fast PWM in PIN5 (OC0B)
//	freq_pwm = F_FPU/((freq_cnt+1)*prescale)
//	duty	 = duty_cnt/freq_cnt
void timer0_fast_pwm(prescale0_t prescale, uint8_t freq_cnt, uint8_t duty_cnt) {
	// clear OC0B on compare match (with OCR0B)
	// fast PWM with OCR0A as TOP
	
	power_timer0_enable();
	TCNT0 	= 0;
    
	OCR0A 	= freq_cnt;
	OCR0B 	= duty_cnt;
	TIMSK0 	= 0;
    
	TCCR0A 	= (0x2 << COM0B0) | (0x3 << WGM00);
	TCCR0B 	= (0x1 << WGM02) | (prescale << CS00);
}


// generate phase corect PWM in PIN6  
//	freq_pwm = F_FPU/((2*freq_cnt)*prescale)
//	duty	 = duty_cnt/freq_cnt
void timer0_pcorrect_pwm(prescale0_t prescale, uint8_t freq_cnt, uint8_t duty_cnt) {
	// clear OC0B on compare match (with OCR0B)
	// phase correct PWM with OCR0A as TOP
	power_timer0_enable();
	TCNT0 	= 0;
    
	OCR0A 	= freq_cnt;
	OCR0B 	= duty_cnt;
	TIMSK0 	= 0;
    
	TCCR0A 	= (0x2 << COM0B0) | (0x1 << WGM00);
	TCCR0B 	= (0x1 << WGM02) | (prescale << CS00);
}


void timer0_stop() {
	power_timer0_disable();
}


void timer1_ctc(prescale1_t prescale, uint16_t top_cnt) {
    
    power_timer1_enable();
    
    TCNT1 = 0;
    
    OCR1A = top_cnt;
    TIMSK1 = _BV(OCIE1A);

    TCCR1A = 0;
    TCCR1B = (0x1 << WGM12) | (prescale << CS10);
}

void timer1_ctc_extra_interrupt(uint16_t interrupt_cnt) {
    OCR1B = interrupt_cnt;
    TIMSK1 |= _BV(OCIE1B);
}

void timer1_ctc_set_interrupt_cnt(uint16_t interrupt_cnt) {
    OCR1B = interrupt_cnt;
}

void timer1_stop() {
	power_timer1_disable();
}


void timer2_ctc(prescale2_t prescale, uint8_t top_cnt) {
    
	power_timer2_enable();
    ASSR    = 0;
	TCNT2 	= 0;
    
	OCR2A 	= top_cnt;
	TIMSK2 	= _BV(OCIE2A);
    
	TCCR2A 	= (0x2 << WGM20);
	TCCR2B 	= (prescale << CS20);

}

void timer2_ctc_extra_interrupt(uint8_t interrupt_cnt) {
    OCR2B = interrupt_cnt;
    TIMSK2 |= _BV(OCIE2B);
}


void timer2_ctc_set_interrupt_cnt(uint8_t interrupt_cnt) {
    OCR2B = interrupt_cnt;
}

void timer2_stop() {
	power_timer2_disable();
}
