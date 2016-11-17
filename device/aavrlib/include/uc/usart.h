/*******************************************************************************
 *	usart.h
 *
 *  usart
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
 * @file usart.h
 * 
 * @brief USART functionality
 */

// BAUD = Fose/16/(UBBR0+1)
// setting of baud rate UBRR0=103=0x67 9600 baud rate
// setting of baud rate UBRR0=3=0x03   250k baud rate
// setting the frame format 1bit-stop 8bit-data
// enable transmitter or receiver


#ifndef _USART
#define _USART

#include <avr/io.h>
#include <avr/power.h>
#include <stdint.h>


/**
 * @brief Init usart functionality
 * 
 * This function must be called once before using any other usart functions
 */
void usart_init();


/**
 * @brief Send a byte of data via USART
 * 
 * @param data_byte The byte to send
 */
void usart_send(uint8_t data_byte);


/**
 * @brief Print an 8 bit unsigned number
 * 
 * @param number The number to print
 */
void usart_printnumber8(uint8_t number);


/**
 * @brief Print an 32 bit unsigned number
 * 
 * @param number The number to print
 */
void usart_printnumber32(uint32_t number);


/**
 * @brief Print an 32 bit signed number
 * 
 * @param number The number to print
 */
void usart_printsignumber32(int32_t number);


/**
 * @brief Print a string ended in '\0'
 * 
 * @param string The string to print
 */
void usart_print(const char* string);



#endif
