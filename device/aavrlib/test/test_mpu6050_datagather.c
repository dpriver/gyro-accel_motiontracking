/*******************************************************************************
 *  test_mpu6050.c
 *
 *  test fo the mpu6050 munted on the GY521 board
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

#include <util/delay.h>
#include <util/twi.h>
#include <boards/arduinoUNO.h>
#include <uc/system.h>
#include <uc/usart.h>
#include <peripherals/MPU60X0.h>
#include <systick.h>
#include <time.h>

#define GYROX_OFFSET (12)
#define GYROY_OFFSET (-21)
#define GYROZ_OFFSET (34)

#define ACCELX_OFFSET (-993)
#define ACCELY_OFFSET (-222)
#define ACCELZ_OFFSET (1360)

#ifndef GYRO_SENS_SCALE
    #define GYRO_SENS_SCALE MPU60X0_GYRO_SCALE_500dps
#endif

#ifndef ACCEL_SENS_SCALE
    #define ACCEL_SENS_SCALE MPU60X0_ACCEL_SCALE_4G
#endif

#ifndef DLPF_MODE
    #define DLPF_MODE MPU60X0_DLPF_OFF
#endif

#ifndef SMP_DIV
    #define SMP_DIV (23)
#endif

#define ABS(a) ( ((a) < 0) ? -(a) : (a))

int main( void ) {

    mpu60x0_sens_t mpu_data[100];
    mpu60x0_state mpu60x0_error;
    int16_t read_values = 0;
    time_t time;
    uint16_t end_time;
    
    system_init();
    systick_init();
    usart_init();
    
    delay_ms(100);
    usart_print("# MPU6050 TEST\n#\n");
    usart_print("# Gyro Sensitivity: ");
    usart_printnumber8(GYRO_SENS_SCALE);
    usart_print("\n# Accel Sensitivity: ");
    usart_printnumber8(ACCEL_SENS_SCALE);
    usart_print("\n# DLPF: ");
    usart_printnumber8(DLPF_MODE);
    usart_print("\n# Sample Rate Divider: ");
    usart_printnumber8(SMP_DIV);
    
    mpu60x0_error = mpu60x0_init(GYRO_SENS_SCALE, ACCEL_SENS_SCALE, DLPF_MODE, SMP_DIV);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        usart_print("#MPU6050 is not working... CODE: ");
        usart_printnumber8(mpu60x0_error);
        return 0;
    }
    else {
        usart_print("\n Initialized mpu6050\n");
    }    

    
    // wait 30 seconds to stabilice the sensors
    delay_ms(1000);
    
    // Set calibration offsets
    mpu60x0_set_gyro_bias(GYROX_OFFSET, GYROY_OFFSET, GYROZ_OFFSET);
    mpu60x0_set_accel_bias(ACCELX_OFFSET, ACCELY_OFFSET, ACCELZ_OFFSET);
    
    // reset fifo
    mpu60x0_flush();
        
    usart_print("\n# TIME      GYROx       GYROy       GYROx       ACCELx");
    usart_print("      ACCELy      ACCELz\n");
    
    
    // gather data for 20 seconds
    get_uptime(&time);
    end_time = time.ms + 20000;
    while(time.ms < end_time) {
        uint16_t i = 0;
        
        read_values = mpu60x0_read_fifo(mpu_data, 100);
        
        if (read_values < 0 ) {
            usart_print("Error ");
            usart_printsignumber32(read_values);
            usart_print("\n");
            continue;     
        }
        
        for (i = 0 ; i < read_values ; i++) {
            usart_print("\n");
            usart_printnumber32(time.ms);
            
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].gyro.x);
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].gyro.y);
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].gyro.z);
            
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].accel.x);
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].accel.y);
            usart_print(" ");
            usart_printsignumber32(mpu_data[i].accel.z);
        }
        
        get_uptime(&time);
    }
    
    while(1);
    
    
    return 0;
}
