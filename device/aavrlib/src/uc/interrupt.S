/*******************************************************************************
 *	interrupt.S
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

#include <avr/io.h>

.section .section, "ax", @progbits

.global __vector_default
.global interrupt_vector_start

.org 0x0

interrupt_vector_start:
    jmp interrupt_vector_start
    jmp _interrupt_0
    jmp _interrupt_1

__vector_default:
    jmp __init

_interrupt_0:
    in  r0,  0x3f
    lds r30, 0x0130
    lds r31, 0x0131
    ldi r24, 0x18
    ldi r25, 0x00
    icall
    out 0x3f, r0
    reti

_interrupt_1:
    in  r0,  0x3f
    lds r30, 0x0130
    lds r31, 0x0131
    ldi r24, 0x18
    ldi r25, 0x00
    icall
    out 0x3f, r0
    reti
