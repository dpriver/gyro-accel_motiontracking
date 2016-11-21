/*******************************************************************************
 *	MPU60X0.h
 *
 *  MPU60X0 gyroscope-accelerometer sensor
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

#ifndef __MPU_60X0
#define __MPU_60X0

#define MPU60X0_ENGINEERING_SAMPLE_VERSION
#define MPU60X0_I2C_ADDR    (0x68)


/* MPU60X0 Register */
#define MPU60X0_REG_ACCEL_BIASX     (0x06)
#define MPU60X0_REG_ACCEL_BIASY     (0x08)
#define MPU60X0_REG_ACCEL_BIASZ     (0x0A)
#define MPU60X0_REG_GYRO_BIASX      (0x13)
#define MPU60X0_REG_GYRO_BIASY      (0x15)
#define MPU60X0_REG_GYRO_BIASZ      (0x17)
#define MPU60X0_REG_SELF_TEST       (13)
#define MPU60X0_REG_SAMPLE_RATE     (25)
#define MPU60X0_REG_CONFIG          (26)
#define MPU60X0_REG_GYRO_CONF       (27)
#define MPU60X0_REG_ACCEL_CONF      (28)
#define MPU60X0_REG_FIFO_EN         (35)
#define MPU60X0_REG_I2C_MASTER_CTL  (36)
#define MPU60X0_REG_I2C_SLAVE0_CTL  (37)
#define MPU60X0_REG_I2C_SLAVE1_CTL  (40)
#define MPU60X0_REG_I2C_SLAVE2_CTL  (43)
#define MPU60X0_REG_I2C_SLAVE3_CTL  (46)
#define MPU60X0_REG_I2C_SLAVE4_CTL  (49)
#define MPU60X0_REG_MASTER_STATUS   (54)
#define MPU60X0_REG_INT_CONF        (55)
#define MPU60X0_REG_INT_EN          (56)
#define MPU60X0_REG_INT_STATUS      (58)
#define MPU60X0_REG_I2C_SLAVE0_DATA     (99)
#define MPU60X0_REG_I2C_SLAVE1_DATA     (100)
#define MPU60X0_REG_I2C_SLAVE2_DATA     (101)
#define MPU60X0_REG_I2C_SLAVE3_DATA     (102)
#define MPU60X0_REG_I2C_MASTER_DELAY    (103)
#define MPU60X0_REG_SIG_PATH_RST    (104)
#define MPU60X0_REG_USER_CTL        (106)
#define MPU60X0_REG_POWER1          (107)
#define MPU60X0_REG_POWER2          (108)
#define MPU60X0_REG_FIFO_COUNT      (114)
#define MPU60X0_REG_FIFO_DATA       (116)
#define MPU60X0_REG_WHO_AM_I        (117)

#define MPU60X0_REG_ACCEL_DATAX      (59)
#define MPU60X0_REG_ACCEL_DATAY      (51)
#define MPU60X0_REG_ACCEL_DATAZ      (63)
#define MPU60X0_REG_GYRO_DATAX       (67)
#define MPU60X0_REG_GYRO_DATAY       (69)
#define MPU60X0_REG_GYRO_DATAZ       (71)
#define MPU60X0_REG_TEMP_DATA       (65)
#define MPU60X0_REG_EXTERN_DATA     (73)

#ifdef MPU60X0_ENGINEERING_SAMPLE_VERSION
    typedef enum {
        MPU60X0_ACCEL_SCALE_4G  = 0,
        MPU60X0_ACCEL_SCALE_8G  = (0x1 << 3),
        MPU60X0_ACCEL_SCALE_16G = (0x2 << 3),
        MPU60X0_ACCEL_SCALE_32G = (0x3 << 3)
    } mpu60x0_accel_scale_t;
#else
    typedef enum {
        MPU60X0_ACCEL_SCALE_2G  = 0,
        MPU60X0_ACCEL_SCALE_4G  = (0x1 << 3),
        MPU60X0_ACCEL_SCALE_8G  = (0x2 << 3),
        MPU60X0_ACCEL_SCALE_16G = (0x3 << 3)
    } mpu60x0_accel_scale_t;
#endif

typedef enum {
    MPU60X0_GYRO_SCALE_250dps  = 0,
    MPU60X0_GYRO_SCALE_500dps  = (0x1 << 3),
    MPU60X0_GYRO_SCALE_1000dps = (0x2 << 3),
    MPU60X0_GYRO_SCALE_2000dps = (0x3 << 3)
} mpu60x0_gyro_scale_t;

typedef enum {
    MPU60X0_DLPF_OFF   = 0,
    MPU60X0_DLPF_184Hz = 0x1,
    MPU60X0_DLPF_94Hz  = 0x2,
    MPU60X0_DLPF_44Hz  = 0x3,
    MPU60X0_DLPF_21Hz  = 0x4,
    MPU60X0_DLPF_10Hz  = 0x5,
    MPU60X0_DLPF_5Hz   = 0x6
} mpu60x0_dlpf_mode_t;

typedef enum {
    MPU60X0_REG_ACCEL_BIAS = MPU60X0_REG_ACCEL_BIASX,
    MPU60X0_REG_GYRO_BIAS  = MPU60X0_REG_GYRO_BIASX,
    MPU60X0_REG_ACCEL_DATA = MPU60X0_REG_ACCEL_DATAX,
    MPU60X0_REG_GYRO_DATA  = MPU60X0_REG_GYRO_DATAX
} mpu60x0_data_reg;


typedef enum {
    MPU60X0_SUCCESS      =  0,
    MPU60X0_I2C_ERR      = -1,     // generic TWI/I2C error
    MPU60X0_WRITE_ERR    = -2,     // error at write checking
    MPU60X0_READ_ERR     = -3,     // error at reading
    MPU60X0_IDENTITY_ERR = -4      // error at identity checking
} mpu60x0_state;


typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mpu60x0_data_t;

typedef struct {
    int16_t temp;
} mpu60x0_temp_data_t;

typedef struct {
    mpu60x0_data_t accel;
    mpu60x0_data_t gyro;
} mpu60x0_sens_t;


/* Configure all the chip registers to use the on-chip FIFO so this driver can 
 * burst read the FIFO register instead of reading all the registers 
 * secuentially */
mpu60x0_state mpu60x0_init(mpu60x0_gyro_scale_t gyro_scale, mpu60x0_accel_scale_t accel_scale, 
                    mpu60x0_dlpf_mode_t dlpf_mode, uint8_t smp_div);

/*
 * Read from MPU registers
 * More than one register can be read in one call, as long as they are consecutive,
 * indicating a length of more than 1.
 */
mpu60x0_state mpu60x0_read_reg(uint8_t reg, uint8_t *data, uint8_t length);

/*
 * Write to MPU registers
 * More than one register can be written in one call, as long as they are consecutive,
 * indicating a length of more than 1.
 */
mpu60x0_state mpu60x0_write_reg(uint8_t reg, uint8_t *data, uint8_t length);

/*
 * Read the sensors' data from the internal FIFO
 */
int16_t mpu60x0_read_fifo(mpu60x0_sens_t *data, uint16_t length);

/*
 * Clear the fifo and restart the sensors readings
 */
mpu60x0_state mpu60x0_flush();

/*
 * Read some data registers
 */
mpu60x0_state mpu60x0_read_data(mpu60x0_data_reg data_reg, mpu60x0_data_t* data);

/*
 * Read temperature data
 */
mpu60x0_state mpu60x0_read_temp(mpu60x0_temp_data_t* data);

/*
 * Set the current gyroscope calibration bias 
 */
mpu60x0_state mpu60x0_set_gyro_bias(int16_t x_bias, int16_t y_bias, int16_t z_bias);

/*
 * Set the current accelerometer calibration bias 
 */
mpu60x0_state mpu60x0_set_accel_bias(int16_t x_bias, int16_t y_bias, int16_t z_bias);

#endif /* __MPU_60X0 */
