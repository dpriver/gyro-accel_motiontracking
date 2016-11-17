/*******************************************************************************
 *	twi.h
 *
 *  TWI/I2C related definitions and functions
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


/**
 * @file twi.h
 * @brief TWI/I2C functionality implementation
 * 
 * This file implements the functionality to handle the TWI module included
 * in the AVR microcontroller.
 * 
 * An init function must be called once before using the TWI functionality.
 * The slave operations are pasive, which means that it only respond to
 * requests from other devices in the bus.
 * When working as master, the rest of the functions are available to write or
 * read to/from other devices on the bus. For that, the TWI_read and TWI_write 
 * functions should be used.
 * When TWI_read or TWI_write is called, the bus is granted to the device and 
 * remains occupied until TWI_release is called. read and writes can be chained 
 * whichever way, resulting in an atomic bus operation.
 * 
 * For example, the following code writes some 10 byte data to the bus and then 
 * reads it back before releasing the bus.
 * 
 * TWI_write(DEVICE_ADDR, &data, 10);
 * TWI_read(DEVICE_ADDR, &data, 10);
 * TWI_release();
 * 
 * Some devices e.g. devices with multiple accesible internal registers, have a 
 * specific protocol to read and write. Because of that, the TWI_do_start, 
 * TWI_do_send_addr, TWI_do_write and TWI_do_read functions are provided.
 * These funstions must be called in a specific order (do_start, do_send_addr, 
 * {do_write/do_read}, release)
 * To chain bus operations, the whole sequence must be repeated (excluding the 
 * release)
 * e.g. do_start, do_send_addr, {do_write/do_read}, do_start_do_send_addr, 
 * {do_write/do_read}, ..., release.
 */

#ifndef __TWI
#define __TWI

#include <stdbool.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/twi.h>


/*==============================================================================
 *      TWI constants and types
 *==============================================================================
 */

/**
 * @brief TWI/I2C posible working modes
 */
typedef enum {
    TWI_DISABLED = 0,   //!< TWI/I2C disabled 
    TWI_MASTER,         //!< TWI/I2C working as master device
    TWI_SLAVE           //!< TWI/I2C working as slave device
} twi_operation_mode;

/**
 * @brief TWI/I2C operation states
 * 
 * These are the possible result states for the TWI operations
 */
typedef enum {
    TWI_SUCCESS = 0,    //!< TWI/I2C operation finished succesfully
    TWI_ERROR = -1      //!< TWI/I2C operation finished in error
} twi_state;

/**
 * @brief TWI/I2C posible operations
 * 
 * These are the posible TWI operation types as required by the TWI_do_send_addr 
 * function
 */
typedef enum {
    TWI_WRITE = TW_WRITE,   //!< TWI/I2C write operation
    TWI_READ = TW_READ      //!< TWI/I2C read operation
} twi_operation_t;




/*==============================================================================
 *      TWI functions
 *==============================================================================
 */ 
 
/**
 * @brief Init TWI/I2C subsystem as a master device
 * 
 * Working as master allows to usage of the send and receive operations
 */
void TWI_master_init();


/**
 * @brief Init TWI/I2C subsystem as a slave device (NOT USABLE YET)
 * 
 * In slave mode, the device can not send nor receive data actively, just when 
 * required by a bus master. Because of that, the rest of the functions 
 * should not be used.
 * 
 * The device will answer the bus requests via interrupt.
 * 
 * @param addr The slave address of this device
 */
void TWI_slave_init(uint8_t addr);


/**
 * @brief Check if there was an error in the last TWI operation
 * 
 * In addition to return the last error (if any), resets the error state, so a 
 * consecutive call won't report any error.
 * 
 * @param error_code The TWI error code
 * @return Returns a boolean value to indicate if there was an error
 */
bool TWI_has_error(uint8_t *error_code);


/**
 * @brief To do a TWI start operation
 * 
 * Sends a start condition to the bus. Besides using it to start a bus transaction, 
 * it can be used just before a do_write or do_read to chain another operation.
 * 
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_do_start();


/**
 * @brief To do a TWI send address operation
 * 
 * Sends the address of the current request. Must be executed after do_start.
 * 
 * @param slave_addr The slave device address
 * @param twi_operation The TWI operation to perform (write or read)
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_do_send_addr(uint8_t slave_addr, twi_operation_t twi_operation);


/**
 * @brief To do a TWI write operation
 * 
 * Performs a bus write operation. It can be called in a row as much times as 
 * needed.
 * 
 * @param byte The byte to send through TWI
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_do_write(uint8_t byte);


/**
 * @brief To do a TWI read operation
 * 
 * Performs a bus read operation. It can be called in a row as much times as 
 * needed.
 * 
 * @param byte The variable to store the read byte
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_do_read(uint8_t *byte);


/**
 * @brief Send data though TWI as master
 * 
 * Performs a complete multibyte send sequence. Multiple send or receive 
 * operations can be chained as a whole transaction. The bus must be actively 
 * released when the transaction ends, so other masters can use it.
 * 
 * @param slave_addr The slave device address
 * @param data A pointer to the data array to send thought TWI
 * @param data_length The number of bytes to send
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_send(uint8_t slave_addr, const uint8_t* data, uint8_t data_length);


/**
 * @brief Receive data through TWI as master
 * 
 * Performs a complete multibyte receive sequence. Multiple send or receive 
 * operations can be chained as a whole transaction. The bus must be actively 
 * released when the transaction ends, so other masters can use it.
 * 
 * @param slave_addr The slave device address
 * @param data A pointer to store the data received from TWI
 * @param data_length The number of bytes to receive
 * @return Returns the finish state of the TWI operation
 */
twi_state TWI_receive(uint8_t slave_addr, uint8_t* data, uint8_t data_length);


/**
 * @brief Release the TWI channel so other master devices can use it.
 * 
 * This function should be called after every transaction. If not, the bus will 
 * remain occupied and no other master could ever use it.
 */
void TWI_release();


#endif /* __TWI */
