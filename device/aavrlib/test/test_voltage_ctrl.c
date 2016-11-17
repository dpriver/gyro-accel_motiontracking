/*******************************************************************************
 *	lcd_1602a.c
 *
 *  LCD 1602a peripheral
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

/*
 * NOT TESTED!! 
 */
 
#include <boards/arduinoUNO.h>
#include <uc/system.h>
#include <uc/analog.h>
#include <uc/usart.h>
#include <systick.h>
#include <softPWM_long.h>


#define ADC_MASK 0x0

#define PID_KP 0.01
#define PID_KI 0.01
#define PID_KD 0.01


int main( void ) {
    double voltage_reference = 0;
    double voltage_value = 0;
    
    double pid_error = 0;
    double pid_sum = 0;
    double pid_deriv = 0;
    double pid_last_e = 0;
    double pid_control = 0;
    
    
    time_t timeout;
    
    system_init();
    usart_init();
    adc_init(adc_presc_128, adc_ref_vcc, adc_channel_a0, ADC_MASK);
    systick_init();
    softPWM_l_init();
    
    
    softPWM_l_add_signal(PIN_4, &PORT_B, &PORT_B_V, 0, 0);
    
    while (1) {
        start_timeout(20, &timeout);
        
        // analog read of the reference signal
        adc_change_channel(adc_channel_a0);
        voltage_reference = adc_single_read();

        // analog read of the system signal
        adc_change_channel(adc_channel_a1);
        voltage_reference = adc_single_read();

        // calculate error and control
        pid_error = voltage_reference - voltage_value;
        pid_sum += pid_error;
        pid_deriv = pid_last_e - pid_error;
        pid_last_e = pid_error;
        
        pid_control = PID_KP*pid_error + PID_KI*pid_sum + PID_KD*pid_deriv;
        
        
        // control signal boundaries
        if (pid_control < 0) {
            pid_control = 0;
        }
        else if (pid_control > SOFTPWM_L_MAX_WIDTH) { 
            pid_control = SOFTPWM_L_MAX_WIDTH;
        }
        
        // set new PWM duty
        softPWM_l_set_pulse_width(0, (uint8_t)pid_control);
        
        // wait for the new control cycle to start
        while(!timeout_expired(&timeout));
    }

    return 0;
}
