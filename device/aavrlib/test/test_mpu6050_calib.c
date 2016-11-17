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

/*
    mpu60x0_set_accel_bias(-2870, -230, 1388);
    mpu60x0_set_gyro_bias(13, -21, 35);
 */
 
#define ABS(a) ( ((a) < 0) ? -(a) : (a))


void print_bias(mpu60x0_data_t *accel_bias, mpu60x0_data_t *gyro_bias) {
                    
    usart_print("\n [BIAS]\n ----------------------------");
    usart_print("\n GYROx       GYROy       GYROx       ACCELx");
    usart_print("      ACCELy      ACCELz\n");

    usart_print(" ");
    usart_printsignumber32(gyro_bias->x);
    usart_print(" ");
    usart_printsignumber32(gyro_bias->y);
    usart_print(" ");
    usart_printsignumber32(gyro_bias->z);
    usart_print(" ");
    usart_printsignumber32(accel_bias->x);
    usart_print(" ");
    usart_printsignumber32(accel_bias->y);
    usart_print(" ");
    usart_printsignumber32(accel_bias->z);

    usart_print("\n");          
}

void gather_data(int32_t *gyroX_mean, int32_t *gyroY_mean, int32_t *gyroZ_mean, 
                int32_t *accelX_mean, int32_t *accelY_mean, int32_t *accelZ_mean) {
    
    mpu60x0_sens_t mpu_data[100];
    time_t time;
    uint32_t end_time;
    int32_t readings = 0;
    int16_t read_values = 0;
    
    
    // gather data for 30 seconds
    *gyroX_mean = 0;
    *gyroY_mean = 0;
    *gyroZ_mean = 0;
    *accelX_mean = 0;
    *accelY_mean = 0;
    *accelZ_mean = 0;
    
    get_uptime(&time);
    end_time = time.ms + 30000;

    //usart_print("\n Start time: ");
    //usart_printnumber32(time.ms);
    //usart_print("\n [READINGS]\n ----------------------------\n");
    //usart_print("\n GYROx       GYROy       GYROx       ACCELx");
    //usart_print("      ACCELy      ACCELz\n");  
    while(time.ms < end_time) {
        uint16_t i = 0;
        
        read_values = mpu60x0_read_fifo(mpu_data, 100);
        
        if (read_values < 0 ) {
            usart_print("Error ");
            usart_printsignumber32(read_values);
            usart_print("\n");
            continue;     
        }
        
        readings += read_values;
        
        for (i = 0; i < read_values ; i++) {
            
            *gyroX_mean += mpu_data[i].gyro.x;
            *gyroY_mean += mpu_data[i].gyro.y;
            *gyroZ_mean += mpu_data[i].gyro.z;
            *accelX_mean += mpu_data[i].accel.x;
            *accelY_mean += mpu_data[i].accel.y;
            *accelZ_mean += mpu_data[i].accel.z;

            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].gyro.x);
            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].gyro.y);
            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].gyro.z);
            
            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].accel.x);
            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].accel.y);
            //usart_print(" ");
            //usart_printsignumber32(mpu_data[i].accel.z);
            //usart_print("\n");
        }
        
        get_uptime(&time);
    }
    //usart_print("\n End time: ");
    //usart_printnumber32(time.ms);    


    *gyroX_mean /= readings;
    *gyroY_mean /= readings;
    *gyroZ_mean /= readings;
    *accelX_mean /= readings;
    *accelY_mean /= readings;
    *accelZ_mean /= readings;

    //usart_print("\n Num readings: ");
    //usart_printnumber32(readings);
    usart_print("\n [MEAN READINGS]\n ----------------------------");
    usart_print("\n GYROx       GYROy       GYROz       ACCELx ");
    usart_print("      ACCELy      ACCELz\n");

    usart_print(" ");
    usart_printsignumber32(*gyroX_mean);
    usart_print(" ");
    usart_printsignumber32(*gyroY_mean);
    usart_print(" ");
    usart_printsignumber32(*gyroZ_mean);
    
    usart_print(" ");
    usart_printsignumber32(*accelX_mean);
    usart_print(" ");
    usart_printsignumber32(*accelY_mean);
    usart_print(" ");
    usart_printsignumber32(*accelZ_mean);
}


int8_t update_bias(int32_t gyroX_mean, int32_t gyroY_mean, int32_t gyroZ_mean, 
                int32_t accelX_mean, int32_t accelY_mean, int32_t accelZ_mean) {
 
    mpu60x0_data_t gyro_bias;
    mpu60x0_data_t accel_bias;
    
    // print bias
    if (mpu60x0_read_data(MPU60X0_REG_ACCEL_BIAS, &accel_bias) != 0)
        return -1;
    if (mpu60x0_read_data(MPU60X0_REG_GYRO_BIAS, &gyro_bias) != 0)
        return -1;
    
    gyroX_mean = gyro_bias.x - (gyroX_mean/4);
    gyroY_mean = gyro_bias.y - (gyroY_mean/4);
    gyroZ_mean = gyro_bias.z - (gyroZ_mean/4);

    // remove gravity from bias calculation
    accelX_mean = accel_bias.x - ((accelX_mean-16384)/8);
    accelY_mean = accel_bias.y - (accelY_mean/8);
    accelZ_mean = accel_bias.z - (accelZ_mean/8);


    // set new calib values
    mpu60x0_set_accel_bias(accelX_mean, accelY_mean, accelZ_mean);
    mpu60x0_set_gyro_bias(gyroX_mean, gyroY_mean, gyroZ_mean);

    // print bias
    if (mpu60x0_read_data(MPU60X0_REG_ACCEL_BIAS, &accel_bias) != 0)
        return -1;
    if (mpu60x0_read_data(MPU60X0_REG_GYRO_BIAS, &gyro_bias) != 0)
        return -1;
    print_bias(&accel_bias, &gyro_bias);

    return 0;
}


int main( void ) {
        
    uint8_t twi_error;
    int32_t gyroX_mean = 0;
    int32_t gyroY_mean = 0;
    int32_t gyroZ_mean = 0;
    int32_t accelX_mean = 0;
    int32_t accelY_mean = 0;
    int32_t accelZ_mean = 0;
    
    system_init();
    systick_init();
    usart_init();

    delay_ms(5000);
    usart_print("############################################################\n");
    usart_print("# MPU6050 CALIBRATION TEST\n#\n");
    usart_print("# Gyro and Accel Sensitivity: 2G and 250dps");
    usart_print("\n# DLPF: 5Hz");
    usart_print("\n# Sample Rate Divider:");
    usart_printnumber8(7);
    usart_print("\n############################################################\n");
    
    delay_ms(3000);
    if ((twi_error = mpu60x0_init(MPU60X0_GYRO_SCALE_250dps, MPU60X0_ACCEL_SCALE_2G, MPU60X0_DLPF_5Hz, 7)) != 0) {
        usart_print("\n MPU6050 is not working... CODE: ");
        usart_printnumber8(twi_error);
        return 0;
    }
    else {
        usart_print("\n Initialized mpu6050\n");
    }

    usart_print("\n Gathering data...\n");
    mpu60x0_flush();
    gather_data(&gyroX_mean, &gyroY_mean, &gyroZ_mean, &accelX_mean, &accelY_mean, &accelZ_mean);
    
    while((ABS(accelX_mean) > (16384+10)) || (ABS(accelY_mean) > 10) || (ABS(accelZ_mean) > 10) ||
          (ABS(gyroX_mean) > 10) || (ABS(gyroY_mean) > 10) || (ABS(gyroZ_mean) > 10)) {
    
        usart_print("\n ==========================================================================");
        update_bias(gyroX_mean, gyroY_mean, gyroZ_mean, accelX_mean, accelY_mean, accelZ_mean);
        mpu60x0_flush();
        usart_print("\n Gathering data...\n");
        gather_data(&gyroX_mean, &gyroY_mean, &gyroZ_mean, &accelX_mean, &accelY_mean, &accelZ_mean);
    }

    usart_print("\n ==========================================================================");
    usart_print("\n\n Calibration finished ");

    while(1);
    
    
    return 0;
}
