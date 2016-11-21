/*******************************************************************************
 *	MPU60X0.c
 *
 *  MPU-60X0 gyroscope-accelerometer sensor
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

#include "uc/usart.h"
#include <util/twi.h>
#include <util/delay.h>
#include "uc/twi.h"
#include "peripherals/MPU60X0.h"
#include "uc/usart.h"




/*
 * Configure to use FIFO and maybe some interrupts
 * 
 */
mpu60x0_state mpu60x0_init(mpu60x0_gyro_scale_t gyro_scale, mpu60x0_accel_scale_t accel_scale, 
                    mpu60x0_dlpf_mode_t dlpf_mode, uint8_t smp_div) {
    uint8_t data;
    mpu60x0_state mpu60x0_error;
    
    TWI_master_init();
    
    /* Check the chip's identity */
    mpu60x0_read_reg(MPU60X0_REG_WHO_AM_I, &data, 1);
    if (data != MPU60X0_I2C_ADDR) {
        return MPU60X0_IDENTITY_ERR;  /* Failed to verify the chip's identity */
    }
    
    /* MPU6050 configuration
     *
     * The default value for all the registers is 0x00 except for:
     * Register 117 (WHO_AM_I) = 0x68
     * Register 107 (POWER1) = 0x40 
     */

    // reset registers to defaults
    data = 0x80;
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_POWER1, &data, 1);
    if ( mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }

    _delay_ms(100);


    data = 0x01;
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_POWER1, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) 
        return mpu60x0_error;
    
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_POWER1, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) 
        return mpu60x0_error;
    if (data != 0x01) 
        return MPU60X0_WRITE_ERR;

    // sample rate
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_SAMPLE_RATE, &smp_div, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) 
        return mpu60x0_error;
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_SAMPLE_RATE, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    if (data != smp_div)
        return MPU60X0_WRITE_ERR;

    // Digital Low Pass Filter (DLPF)
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_CONFIG, (uint8_t*)&dlpf_mode, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_CONFIG, &data, 1);
    if ( mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    if (data != dlpf_mode)
        return MPU60X0_WRITE_ERR;

    // gyroscope scale range
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_GYRO_CONF, (uint8_t*)&gyro_scale, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_GYRO_CONF, &data, 1);
    if ( mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    if (data != gyro_scale)
        return MPU60X0_WRITE_ERR;
        
    // accelerometer scale range
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_ACCEL_CONF, (uint8_t*)&accel_scale, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_ACCEL_CONF, &data, 1);
    if ( mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    if (data != accel_scale)
        return MPU60X0_WRITE_ERR;
    
    // fifo enabled for gyro and accel
    data = 0x78;
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_FIFO_EN, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_FIFO_EN, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS)
        return mpu60x0_error;
    if (data != 0x78)
        return MPU60X0_WRITE_ERR;

    // fifo clean and reset
    mpu60x0_error = mpu60x0_flush();
    
    return mpu60x0_error;
}


mpu60x0_state mpu60x0_read_reg(uint8_t reg, uint8_t *data, uint8_t length) {
    twi_state twi_error;
    
    if (length == 0) {
        return MPU60X0_READ_ERR;
    }
    
    twi_error = TWI_send(MPU60X0_I2C_ADDR, &reg, 1);
    if (twi_error != TWI_SUCCESS) {
        TWI_release();
        return MPU60X0_I2C_ERR;
    }

    twi_error = TWI_receive(MPU60X0_I2C_ADDR, data, length);
    if (twi_error != TWI_SUCCESS) {
        TWI_release();
        return MPU60X0_I2C_ERR;
    }
    
    TWI_release();
    
    return MPU60X0_SUCCESS;
}


mpu60x0_state mpu60x0_write_reg(uint8_t reg, uint8_t *data, uint8_t length) {
    uint8_t i = 0;
    
    if (TWI_do_start() != TWI_SUCCESS) {
        TWI_release();
        return MPU60X0_I2C_ERR;
    }
    
    // TWI_send() cannot be used here because the first sent byte is not part
    // of the data, but the MPU register address, which makes more sense to
    // have as a separated parameter instead of asking the programmer to 
    // include it at the beggining of the "data" array.
    if (TWI_do_send_addr(MPU60X0_I2C_ADDR, TWI_WRITE) != TWI_SUCCESS) {
        TWI_release();
        return MPU60X0_I2C_ERR;
    }
    
    if (TWI_do_write(reg) != TWI_SUCCESS) {
        TWI_release();
        return MPU60X0_I2C_ERR;
    }
        
    while (i < length) {
        if (TWI_do_write(data[i]) != TWI_SUCCESS) {
            TWI_release();
            return MPU60X0_I2C_ERR;
        }
        i++;
    }
    
    TWI_release();
    
    return MPU60X0_SUCCESS;
}


/* Read the fifo
 * Returns the number of data packets read or a mpu60x0_state error
 */
int16_t mpu60x0_read_fifo(mpu60x0_sens_t *data, uint16_t length) {
    uint16_t fifo_count = 0;
    uint8_t i = 0;
    mpu60x0_state mpu60x0_error;

    // get the number of data values in fifo

    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_FIFO_COUNT, (uint8_t *)&fifo_count, 2);
    if ( mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }
    
    fifo_count = (fifo_count << 8) | (fifo_count >> 8);
    
    fifo_count /= 12;
    if (fifo_count == 0) {
        return 0;
    }
    
    // adjust the length of the data to read
    length = (length < fifo_count)? length : fifo_count;

    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_FIFO_DATA, (uint8_t *)data, 12*length);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }


    while (i < length) {
     
        data[i].gyro.x = (data[i].gyro.x << 8) | ((uint16_t)data[i].gyro.x >> 8);
        data[i].gyro.y = (data[i].gyro.y << 8) | ((uint16_t)data[i].gyro.y >> 8);
        data[i].gyro.z = (data[i].gyro.z << 8) | ((uint16_t)data[i].gyro.z >> 8);

        data[i].accel.x = (data[i].accel.x << 8) | ((uint16_t)data[i].accel.x >> 8);
        data[i].accel.y = (data[i].accel.y << 8) | ((uint16_t)data[i].accel.y >> 8);
        data[i].accel.z = (data[i].accel.z << 8) | ((uint16_t)data[i].accel.z >> 8);
        
        i++;
    }

    return length;
}


mpu60x0_state mpu60x0_flush() {
    uint8_t data;
    mpu60x0_state mpu60x0_error;
    
    // fifo and sensor path reset
    data = 0x05;
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_USER_CTL, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }

    // fifo enable
    data = 0x40;
    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_USER_CTL, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }
    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_USER_CTL, &data, 1);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }
    if (data != 0x40) {
        return MPU60X0_WRITE_ERR;
    }
        
    return MPU60X0_SUCCESS;
}


/* 
 * Read some data registers 
 */
mpu60x0_state mpu60x0_read_data(mpu60x0_data_reg data_reg, mpu60x0_data_t* data) {
    mpu60x0_state mpu60x0_error;

    mpu60x0_error = mpu60x0_read_reg(data_reg, (uint8_t *)data, 6);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }

    data->x = (data->x << 8) | ((uint16_t)data->x >> 8);
    data->y = (data->y << 8) | ((uint16_t)data->y >> 8);
    data->z = (data->z << 8) | ((uint16_t)data->z >> 8);
    
    return MPU60X0_SUCCESS;
}


/*
 * Read temperature data
 */
mpu60x0_state mpu60x0_read_temp(mpu60x0_temp_data_t* data) {
    mpu60x0_state mpu60x0_error;

    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_TEMP_DATA, (uint8_t *)data, 2);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }

    data->temp = (data->temp << 8) | ((uint16_t)data->temp >> 8);
    
    return MPU60X0_SUCCESS;    
}


/*
 * Accel bias is specified in +-8G format, and gyro bias in +-1000dps format.
 * because of that, It is convenient to read the data for calibration in 
 * AFS_SEL = 2.
 */
 
/*
 * Input is LSB in +-1000dps format
 */
mpu60x0_state mpu60x0_set_gyro_bias(int16_t x_bias, int16_t y_bias, int16_t z_bias) {
    uint8_t data[6];
    mpu60x0_state mpu60x0_error;

    // high byte is first in mpu60x0
    data[0] = (uint16_t)x_bias >> 8;
    data[1] = x_bias & 0xff;
    data[2] = (uint16_t)y_bias >> 8;
    data[3] = y_bias & 0xff;
    data[4] = (uint16_t)z_bias >> 8;
    data[5] = z_bias & 0xff;

    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_GYRO_BIASX, data, 6);
    
    return mpu60x0_error;
}


/*
 * Input is LSB in +-8G format
 */
mpu60x0_state mpu60x0_set_accel_bias(int16_t x_bias, int16_t y_bias, int16_t z_bias) {
    uint8_t curr_bias[6];
    uint8_t data[6];
    mpu60x0_state mpu60x0_error;

    mpu60x0_error = mpu60x0_read_reg(MPU60X0_REG_ACCEL_BIASX, (uint8_t*)curr_bias, 6);
    if (mpu60x0_error != MPU60X0_SUCCESS) {
        return mpu60x0_error;
    }    
    
    // high byte is first in mpu60x0
    // preserve first bit of the 3 low bytes
    data[0] = (uint16_t)x_bias >> 8;
    data[1] = (x_bias & 0xFE) | (curr_bias[1] & 0x1);
    data[2] = (uint16_t)y_bias >> 8;
    data[3] = (y_bias & 0xFE) | (curr_bias[3] & 0x1);
    data[4] = (uint16_t)z_bias >> 8;
    data[5] = (z_bias & 0xFE) | (curr_bias[5] & 0x1);

    mpu60x0_error = mpu60x0_write_reg(MPU60X0_REG_ACCEL_BIASX, data, 6);
    
    return mpu60x0_error;
}

