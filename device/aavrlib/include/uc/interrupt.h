/*******************************************************************************
 *	interrupt.h
 *
 *  interrupt management
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
 * @file interrupt.h
 * @brief Interrupt handling implementations
 */

#ifndef __INTERRUPT
#define __INTERRUPT


#define N_INTERRUPTS 25

/**
 * @brief All the interrupts of the atmega328p interrupt vector
 */
typedef enum {
    INT0_int,
    INT1_int,
    PCINT0_int,
    PCINT1_int,
    PCINT2_int,
    WDT_int,
    TIMER2_COMPA_int,
    TIMER2_COMPB_int,
    TIMER2_OVF_int,
    TIMER1_CAPT_int,
    TIMER1_COMPA_int,
    TIMER1_COMPB_int,
    TIMER1_OVF_int,
    TIMER0_COMPA_int,
    TIMER0_COMPB_int,
    TIMER0_OVF_int,
    SPI_STC_int,
    USART_RX_int,
    USART_UDRE_int,
    USART_TX_int,
    ADC_int,
    EE_READY_int,
    ANALOG_COMP_int,
    TWI_int,
    SPM_READY_int
} interrupt_t;


/**
 * @brief This macro is used to define an interrupt routine
 * 
 * @param function The name of the interrupt routine
 */
#define INTERRUPT(function) \
    void function (void) __attribute__((signal, __INTR_ATTRS)); \
    void function (void)


/**
 * @brief The definition of the interrupt routines
 */
typedef void (*isr_function)( void );


/**
 * @brief Initialize the interrupts 
 */
void interrupt_init();


/**
 * @brief Attach a routine to an interrupt
 * 
 * The provided function is attached to the interrupt. Also, the interrupt is 
 * enabled.
 * 
 * @param interrupt The interrupt to which the routine is going to be attached
 * @param isr The function to attach
 */
void interrupt_attach(interrupt_t interrupt, isr_function isr);


/**
 * @brief Detach a routine from an interrupt
 * 
 * The routine attached to the indicated interrupt is detached and the interrupt 
 * is disabled.
 * 
 * @param interrupt The interrupt whose routine is to be detached
 */
void interrupt_detach(interrupt_t interrupt);


#endif /* __INTERRUPT */
