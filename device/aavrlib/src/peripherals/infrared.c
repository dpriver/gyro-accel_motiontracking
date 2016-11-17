/*******************************************************************************
 *	infrared.c
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


#include <avr/interrupt.h>
#include "uc/interrupt.h"
#include "systick.h"
#include "peripherals/infrared.h"
#include "uc/usart.h"


#define IR_BUFF_LENGTH 1

#define EQUALS(value, intervale) ( (value >= intervale*0.8) && \
        (value <= intervale*1.2) )



/*==============================================================================
 *   Protocol state definitions
 *============================================================================*/

#define IDLE_ST 0

/* NEC protocol states */
#define NEC_DATA_ST         1
#define NEC_COMPLETED_ST    2


/*==============================================================================
 *   Protocol intervale definitions
 *============================================================================*/
 
/* NEC protocol intervales */
#define NEC_PULSE (562)

#define NEC_HEAD        (9000+4500)
#define NEC_DATA_0      (NEC_PULSE*2)
#define NEC_DATA_1      (NEC_PULSE*4)
#define NEC_TAIL        NEC_PULSE



/*==============================================================================
 *   Infrared structures
 *============================================================================*/

struct {
    completion_handler completion;
    decode_protocol decode;
    
    uint32_t last_lecture;
    uint8_t decode_state;
    
    uint8_t data_length;
    uint8_t data_start;
    uint8_t data_end;
    uint8_t data_buffer[IR_BUFF_LENGTH];
    uint8_t addr_buffer[IR_BUFF_LENGTH];
    
    uint32_t data;
    uint8_t index;
} ir_data;


/*==============================================================================
 *   Infrared ISR
 *============================================================================*/

INTERRUPT(__vector_ir_receiver_isr) {
	uint32_t timevalue, intervale;
    uint8_t oldSREG;
    oldSREG = SREG;

    timevalue = get_micros();

    cli();
    

    if(timevalue > ir_data.last_lecture)
		intervale = timevalue - ir_data.last_lecture;
	else
		intervale = MAX_MICROS_COUNT - ir_data.last_lecture + timevalue;
        
    //time[debug_index] = timevalue;
    //diff[debug_index] = intervale;

    ir_data.decode(intervale);
    ir_data.last_lecture = timevalue;

    SREG = oldSREG;
}



/*==============================================================================
 *   Infrared functions
 *============================================================================*/

void ir_receiver_init(completion_handler completion, decode_protocol decode){

    //int i;
    //debug_index = 0;
    //for(i = 0 ; i < DEBUG_VALS; i++) {
    //    milis[i] = 0;
    //    micros[i] = 0;
    //    diff[i] = 0;
    //    time[i] = 0;
    //}
    
    ir_data.decode = decode;
    ir_data.completion = completion;
    
    ir_data.last_lecture = 0;
    ir_data.decode_state = IDLE_ST;
    
    ir_data.data_length = 0;
    ir_data.data_start = 0;
    ir_data.data_end = 0;
    
    ir_data.index = 0;
    
    interrupt_attach(INT0_int, __vector_ir_receiver_isr);

	EICRA = (EICRA | _BV(ISC01)) & ~_BV(ISC00);
	//EICRA = (EICRA | _BV(ISC00)) & ~_BV(ISC01);
    EIMSK |= _BV(INT0);
    
    // configure pin 2 as input
    DDRD |= _BV(PORTD2);
}


uint8_t nec_decode(uint32_t interval) {
    uint8_t data, addr;
    //uint8_t data_red, addr_red;

    //usart_print("\ninterval: ");
    //usart_printnumber32(interval);

    switch(ir_data.decode_state) {
        case NEC_DATA_ST:
            //usart_print("\t[DATA]");
            if (EQUALS(interval, NEC_DATA_0)) {                 // DATA 0 correct
                ir_data.index++;
                if (ir_data.index == 32) {
                    ir_data.decode_state = NEC_COMPLETED_ST;
                }
            }
            else if (EQUALS(interval, NEC_DATA_1)) {            // DATA 1 correct
                ir_data.data |= 0x80000000 >> ir_data.index;
                ir_data.index++;
                if (ir_data.index == 32) {
                    ir_data.decode_state = NEC_COMPLETED_ST;
                }
            }
            else {                                             // DATA fail
                ir_data.decode_state = IDLE_ST;
            } 
            break;

        case IDLE_ST:
            //usart_print("\t[IDLE]");
            if (EQUALS(interval, NEC_HEAD)) {           // HEAD correct
                ir_data.index = 0;
                ir_data.data = 0;
                ir_data.decode_state = NEC_DATA_ST;
            }
            break; 
    }
    
    if (ir_data.decode_state == NEC_COMPLETED_ST) {
        //usart_print("\t[COMPLETED]");
        //if (EQUALS(interval, NEC_TAIL)) {
        #warning "[TODO] NEC ir protocol: Not checking TAIL"
            addr = (ir_data.data >> 24) & 0xFF;
            //addr_red = ~(ir_data.data >> 16) & 0xFF;
            data = (ir_data.data >> 8) & 0xFF;
            //data_red = ~(ir_data.data) & 0xFF;                
            
           //if ((addr == addr_red) && (data == data_red)) {
           #warning "[TODO] NEC ir protocol: Not checking redundancy"
               ir_data.addr_buffer[ir_data.data_end] = addr;
               ir_data.data_buffer[ir_data.data_end] = data;
               if (ir_data.completion != 0) {
                   ir_data.completion(addr, data);
               }
           //}
        //}
        ir_data.decode_state = IDLE_ST;
    }

    return 0;
}
