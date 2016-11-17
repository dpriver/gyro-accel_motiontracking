/*******************************************************************************
 *	spi.h
 *
 *  spi
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
#include <avr/interrupt.h>
#include <stdint.h>

#include "uc/interrupt.h"
#include "uc/spi.h"
#include "common.h"



volatile static struct{
	uint8_t buffer[1<<8];
	uint8_t length;
	uint8_t last;
	uint8_t sending;
	transsmision_end_handler end_handler;
}spi_attr;


void isr_send_handler(){
	spi_attr.last++;
	spi_sendbyte(spi_attr.buffer[spi_attr.last]);
}

void isr_receive_handler(){
	spi_attr.last++;
	spi_readbyte(spi_attr.buffer[spi_attr.last]);
	spi_receivebyte();
}

INTERRUPT(__vector_spi_stc_isr) {
	if( spi_attr.last < spi_attr.length-1 ){
		if(spi_attr.sending == 1)
			isr_send_handler();
		else
			isr_receive_handler();
	}
	else{
		if(spi_attr.sending == 0)	// I don't like this, but we have to read the last value
			spi_readbyte(spi_attr.buffer[spi_attr.last]);

		spi_end_transmission();
		if(spi_attr.end_handler != NULL){
			sei();						// enable interrupts beyong this point
			spi_attr.end_handler();
		}
	}
}


void spi_init() {
    interrupt_attach(SPI_STC_int, __vector_spi_stc_isr);
}

void spi_send_block(uint8_t caracters[], uint8_t length){
	spi_setup_master();
	spi_begin_transmission();
	for(int i = 0; i<length ; i++){
		spi_sendbyte(caracters[i]);
		while(!spi_finished_transmission());
	}
	spi_end_transmission();
}


void spi_send_burst(uint8_t characters[], uint8_t length, transsmision_end_handler end_handler){
	for(int i = 0; i < length; i++)
		spi_attr.buffer[i] = characters[i];

	spi_attr.length = length;
	spi_attr.last = 0;
	spi_attr.end_handler = end_handler;
	spi_attr.sending = 1;
	if(spi_attr.length > spi_attr.last){
		spi_setup_master_with_int();
		spi_begin_transmission();
		spi_sendbyte(spi_attr.buffer[0]);
	}
	else if(end_handler != NULL)
		end_handler();
}

