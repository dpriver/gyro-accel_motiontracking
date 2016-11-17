/*******************************************************************************
 *	infrared.h
 *
 *  infrared receiver
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

#ifndef __INFRARED
#define __INFRARED

#define DEBUG_VALS 60



typedef void (*completion_handler)(uint8_t address, uint8_t command);
typedef uint8_t (*decode_protocol)(uint32_t interval);

void ir_receiver_init(completion_handler handler, decode_protocol decode);

uint8_t nec_decode(uint32_t interval);

#endif /* __INFRARED */
