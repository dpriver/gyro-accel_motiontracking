/*******************************************************************************
 *	interrupt.c
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

#include <avr/interrupt.h>
#include "uc/interrupt.h"
#include "uc/usart.h"

//#define __STARTFILES

#define N_INTERRUPTS 25

// Declare manually (in assembly) the interrupt vector and the calling routine
// ISR vectors

__attribute__((section(".def_isr"))) void default_ISR( void ) {
    //asm volatile("rjmp interrupt_vector");
}


isr_function isr[N_INTERRUPTS] = {
    default_ISR, default_ISR, default_ISR, default_ISR, default_ISR,
    default_ISR, default_ISR, default_ISR, default_ISR, default_ISR,
    default_ISR, default_ISR, default_ISR, default_ISR, default_ISR,
    default_ISR, default_ISR, default_ISR, default_ISR, default_ISR,
    default_ISR, default_ISR, default_ISR, default_ISR, default_ISR
    };

__attribute__((naked, section(".vectors"))) void interrupt_vector(void) {
    asm volatile(
        "jmp __ctors_end \n\t"
        "jmp __INT0_isr \n\t"
        "jmp __INT1_isr \n\t"
        "jmp __PCINT0_isr \n\t"
        "jmp __PCINT1_isr \n\t"
        "jmp __PCINT2_isr \n\t"
        "jmp __WDT_isr \n\t"
        "jmp __TIMER2_COMPA_isr \n\t"
        "jmp __TIMER2_COMPB_isr \n\t"
        "jmp __TIMER2_OVF_isr \n\t"
        "jmp __TIMER1_CAPT_isr \n\t"
        "jmp __TIMER1_COMPA_isr \n\t"
        "jmp __TIMER1_COMPB_isr \n\t"
        "jmp __TIMER1_OVF_isr \n\t"
        "jmp __TIMER0_COMPA_isr \n\t"
        "jmp __TIMER0_COMPB_isr \n\t"
        "jmp __TIMER0_OVF_isr \n\t"
        "jmp __SPI_STC_isr \n\t"
        "jmp __USART_RX_isr \n\t"
        "jmp __USART_UDRE_isr \n\t"
        "jmp __USART_TX_isr \n\t"
        "jmp __ADC_isr \n\t"
        "jmp __EE_READY_isr \n\t"
        "jmp __ANALOG_COMP_isr \n\t"
        "jmp __TWI_isr \n\t"
        "jmp __SPM_READY_isr \n\t"
        
        "__ctors_end: \n\t"
        "eor	r1, r1  \n\t"
        "out	0x3f, r1	; 63  \n\t"
        "ldi	r28, 0xFF	; 255  \n\t"
        "ldi	r29, 0x08	; 8  \n\t"
        "out	0x3e, r29	; 62  \n\t"
        "out	0x3d, r28	; 61  \n\t"
        
    );
}

__attribute__((naked, section(".init9"))) void main_jump (void) {
    asm volatile (
        "jmp main \n\t"
        "rjmp -2\n\t"
    );
}

__attribute__((section(".def_isr"))) void __isr(void) {
    unsigned int var;

    asm volatile(
        "__INT0_isr: \n\t"
        "sei \n\t"
        "push r30 \n\t"
        "push r31 \n\t"
        "lds r30, %0 \n\t"
        "lds r31, %0 + 1 \n\t"
        "rjmp __isr_call    \n\t"
        "__INT1_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %1        \n\t"
        "lds r31, %1 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__PCINT0_isr:      \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %2        \n\t"
        "lds r31, %2 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__PCINT1_isr:      \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %3        \n\t"
        "lds r31, %3 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__PCINT2_isr:      \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %4        \n\t"
        "lds r31, %4 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__WDT_isr:         \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %5        \n\t"
        "lds r31, %5 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER2_COMPA_isr: \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %6        \n\t"
        "lds r31, %6 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER2_COMPB_isr: \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %7        \n\t"
        "lds r31, %7 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER2_OVF_isr:  \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %8        \n\t"
        "lds r31, %8 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER1_CAPT_isr: \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %9        \n\t"
        "lds r31, %9 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER1_COMPA_isr: \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %10        \n\t"
        "lds r31, %10 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER1_COMPB_isr:  \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %11        \n\t"
        "lds r31, %11 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER1_OVF_isr:  \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %12        \n\t"
        "lds r31, %12 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER0_COMPA_isr:  \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %13        \n\t"
        "lds r31, %13 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER0_COMPB_isr: \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %14        \n\t"
        "lds r31, %14 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TIMER0_OVF_isr:   \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %15        \n\t"
        "lds r31, %15 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__SPI_STC_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %16        \n\t"
        "lds r31, %16 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__USART_RX_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %17        \n\t"
        "lds r31, %17 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__USART_UDRE_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %18        \n\t"
        "lds r31, %18 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__USART_TX_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %19        \n\t"
        "lds r31, %19 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__ADC_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %20        \n\t"
        "lds r31, %20 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__EE_READY_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %21        \n\t"
        "lds r31, %21 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "\n\t"
        "__ANALOG_COMP_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %22        \n\t"
        "lds r31, %22 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__TWI_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %23        \n\t"
        "lds r31, %23 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__SPM_READY_isr:        \n\t"
        "sei                \n\t"
        "push r30           \n\t"
        "push r31           \n\t"
        "lds r30, %24        \n\t"
        "lds r31, %24 + 1    \n\t"
        "rjmp __isr_call    \n\t"
        "__isr_call:        \n\t"
        "icall              \n\t"
        "pop r31            \n\t"
        "pop r30            \n\t"
        "ret                \n\t"
        :
        : "i"(&isr[INT0_int]), "i"(&isr[INT1_int]), "i"(&isr[PCINT0_int]), 
          "i"(&isr[PCINT1_int]), "i"(&isr[PCINT2_int]), "i"(&isr[WDT_int]), 
          "i"(&isr[TIMER2_COMPA_int]), "i"(&isr[TIMER2_COMPB_int]), "i"(&isr[TIMER2_OVF_int]),
          "i"(&isr[TIMER1_CAPT_int]), "i"(&isr[TIMER1_COMPA_int]), "i"(&isr[TIMER1_COMPB_int]),
          "i"(&isr[TIMER1_OVF_int]), "i"(&isr[TIMER0_COMPA_int]), "i"(&isr[TIMER0_COMPB_int]),
          "i"(&isr[TIMER0_OVF_int]), "i"(&isr[SPI_STC_int]), "i"(&isr[USART_RX_int]),
          "i"(&isr[USART_UDRE_int]), "i"(&isr[USART_TX_int]), "i"(&isr[ADC_int]), "i"(&isr[EE_READY_int]),
          "i"(&isr[ANALOG_COMP_int]), "i"(&isr[TWI_int]), "i"(&isr[SPM_READY_int])
    );

}

void interrupt_init() {
    uint8_t i;
    
    for (i = 0; i < N_INTERRUPTS; i++) {
        isr[i] = default_ISR;
    }
}

void interrupt_attach(interrupt_t interrupt, isr_function new_isr) {
    isr[interrupt] = new_isr;
}

void interrupt_detach(interrupt_t interrupt) {
    isr[interrupt] = default_ISR;
}

 
//ISR(INT0_vect, ISR_BLOCK) {
    //isr[INT0_int]();
//}


//ISR(INT1_vect, ISR_BLOCK) {
    //isr[INT1_int]();
//}

//ISR(PCINT0_vect, ISR_BLOCK) {
    //isr[PCINT0_int]();
//}

//ISR(PCINT1_vect, ISR_BLOCK) {
    //isr[PCINT1_int]();
//}

//ISR(PCINT2_vect, ISR_BLOCK) {
    //isr[PCINT2_int]();
//}

//ISR(WDT_vect, ISR_BLOCK) {
    //isr[WDT_int]();
//}

//ISR(TIMER2_COMPA_vect, ISR_BLOCK) {
    //isr[TIMER2_COMPA_int]();
//}

//ISR(TIMER2_COMPB_vect, ISR_BLOCK) {
    //isr[TIMER2_COMPB_int]();
//}

//ISR(TIMER2_OVF_vect, ISR_BLOCK) {
    //isr[TIMER2_OVF_int]();
//}

//ISR(TIMER1_CAPT_vect, ISR_BLOCK) {
    //isr[TIMER1_CAPT_int]();
//}

//ISR(TIMER1_COMPA_vect, ISR_BLOCK) {
    //isr[TIMER1_COMPA_int]();
//}

//ISR(TIMER1_COMPB_vect, ISR_BLOCK) {
    //isr[TIMER1_COMPB_int]();
//}

//ISR(TIMER1_OVF_vect, ISR_BLOCK) {
    //isr[TIMER1_OVF_int]();
//}

//ISR(TIMER0_COMPA_vect, ISR_BLOCK) {
    //isr[TIMER0_COMPA_int]();
//}

//ISR(TIMER0_COMPB_vect, ISR_BLOCK) {
    //isr[TIMER0_COMPB_int]();
//}

//ISR(TIMER0_OVF_vect, ISR_BLOCK) {
    //isr[TIMER0_OVF_int]();
//}

//ISR(SPI_STC_vect, ISR_BLOCK) {
    //isr[SPI_STC_int]();
//}

//ISR(USART_RX_vect, ISR_BLOCK) {
    //isr[USART_RX_int]();
//}

//ISR(USART_UDRE_vect, ISR_BLOCK) {
    //isr[USART_UDRE_int]();
//}

//ISR(USART_TX_vect, ISR_BLOCK) {
    //isr[USART_TX_int]();
//}

//ISR(ADC_vect, ISR_BLOCK) {
    //isr[ADC_int]();
//}

//ISR(EE_READY_vect, ISR_BLOCK) {
    //isr[EE_READY_int]();
//}

//ISR(ANALOG_COMP_vect, ISR_BLOCK) {
    //isr[ANALOG_COMP_int]();
//}

//ISR(TWI_vect, ISR_BLOCK) {
    //isr[TWI_int]();
//}

//ISR(SPM_READY_vect, ISR_BLOCK) {
    //isr[SPM_READY_int]();
//}
