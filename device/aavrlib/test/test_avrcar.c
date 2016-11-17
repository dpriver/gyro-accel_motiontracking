/*******************************************************************************
 *	test_avrcar.c
 *
 *  AVR powered car test
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
#include <stdint.h>
#include <uc/system.h>
#include <uc/usart.h>
#include <peripherals/infrared.h>
#include <peripherals/HCSR04.h>
#include <boards/arduinoUNO.h>
#include <systick.h>
#include <softPWM_long.h>

 
volatile uint8_t last_command = 0; 

// ultrasonic related configuration
#define PORT_CONF_TRIGGER    PORT_C
#define PORT_CONF_ECHO       PORT_C
#define PORT_VAL_TRIGGER     PORT_C_V
#define PORT_VAL_ECHO        PORT_C_R

#define PIN_TRIGGER          PIN_8
#define PIN_ECHO             PIN_9


// Wheel related configuration
#define PIN_LEFT_WHEEL_F   PIN_6
#define PIN_LEFT_WHEEL_B   PIN_5
#define PIN_RIGHT_WHEEL_F  PIN_4
#define PIN_RIGHT_WHEEL_B  PIN_3

#define PORT_CONF_WHEELS   PORT_B
#define PORT_VAL_WHEELS    PORT_B_V

#define SLOT_LEFT_WHEEL_F  0
#define SLOT_LEFT_WHEEL_B  1
#define SLOT_RIGHT_WHEEL_F 2
#define SLOT_RIGHT_WHEEL_B 3


void ir_handler(uint8_t address, uint8_t command);
void behaviour_stop_when_barrier();
void behaviour_user_command();
 
int main (void) {

    system_init();
    systick_init();
    usart_init();
    ultrasonic_init();
    ir_receiver_init(ir_handler, nec_decode);
    
    softPWM_l_init();
    usart_print("AVR car\n=======================================\n\n");


    usart_print("Shuting off arduino's led\n");

    // Config pinA5 as output to signal when marching back
    IOPORT_CONFIG(OUTPUT, PORT_A, _BV(PIN_A5));
    IOPORT_VALUE(LOW, PORT_A, _BV(PIN_A5));


    usart_print("Configuring ultrasonic pins");
    IOPORT_CONFIG(OUTPUT, PORT_CONF_TRIGGER, _BV(PIN_TRIGGER));
    IOPORT_CONFIG(INPUT, PORT_CONF_ECHO, _BV(PIN_ECHO));


    usart_print("Configurating PWM signals\n");
    // configurate PWM signals
    softPWM_l_add_signal(PIN_LEFT_WHEEL_F, &PORT_CONF_WHEELS, &PORT_VAL_WHEELS, 
            SLOT_LEFT_WHEEL_F, 0);
    softPWM_l_add_signal(PIN_LEFT_WHEEL_B, &PORT_CONF_WHEELS, &PORT_VAL_WHEELS, 
            SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_add_signal(PIN_RIGHT_WHEEL_F, &PORT_CONF_WHEELS, &PORT_VAL_WHEELS, 
            SLOT_RIGHT_WHEEL_F, 0);
    softPWM_l_add_signal(PIN_RIGHT_WHEEL_B, &PORT_CONF_WHEELS, &PORT_VAL_WHEELS, 
            SLOT_RIGHT_WHEEL_B, 0);

    usart_print("Starting program...\n");

    while (1) {
        behaviour_user_command();
    }

    return 0;
}


void ir_handler(uint8_t address, uint8_t command) {
    usart_print("[IR received]: ");
    usart_printnumber8(command);
    usart_print("\n");
    last_command = command;
}



void pivote_right() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);    
}

void pivote_left() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);
}

void turn_right() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, SOFTPWM_L_MAX_WIDTH);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);    
}

void turn_left() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, SOFTPWM_L_MAX_WIDTH);
}

void go_front() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);      
}

void go_back() {
    IOPORT_VALUE(HIGH, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, SOFTPWM_L_MAX_WIDTH);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, SOFTPWM_L_MAX_WIDTH);
}

void stop() {
    IOPORT_VALUE(LOW, PORT_B, _BV(PIN_2));
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
    softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);
    softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);      
}


void behaviour_stop_when_barrier() {

    uint16_t distance;

    distance = ultrasonic_measure(&PORT_VAL_TRIGGER, _BV(PIN_TRIGGER), 
                    &PORT_VAL_ECHO, _BV(PIN_ECHO));
  
    if (distance < 0 ) {
        usart_print("Distance measure timeout\n");
        return;
    }
  
    usart_print("distance: ");
    usart_printnumber32(distance);
    usart_print("\n");
    
    if ( distance > 100 ) {
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH);
    } 
    else if (distance > 60 ) {
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH-15);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH-15);                
    }
    else if (distance > 40 ) {
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, SOFTPWM_L_MAX_WIDTH-25);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, SOFTPWM_L_MAX_WIDTH-25);                
    }
    else if (distance < 20 ) {
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, SOFTPWM_L_MAX_WIDTH-25);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, SOFTPWM_L_MAX_WIDTH-25);
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);           
    }
    else { /*  10 < distance < 20 */
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_B, 0);
        softPWM_l_set_pulse_width(SLOT_LEFT_WHEEL_F, 0);
        softPWM_l_set_pulse_width(SLOT_RIGHT_WHEEL_F, 0);                   
    }
    
    delay_ms(200);
}

void behaviour_user_command() {
    switch(last_command) {
        case 0: 
            stop();
            break;
        case 32:
            turn_left();
            break;
        case 96:
            turn_right();
            break;
        case 16:
            go_back();
            break;
        case 80:
            go_front();
            break;
    }
}
