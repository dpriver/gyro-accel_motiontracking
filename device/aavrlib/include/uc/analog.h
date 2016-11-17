/*******************************************************************************
 *	analog.h
 *
 *  analog comparator and converter
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
 ********************************************************************************/

/**
 * @file analog.h
 * @brief Analog I/O functionality
 * 
 * This file contains the functionality for analog I/O.
 * Analog I/O is made thoughr Analog-to-Digital Conversors (ADC) connected
 * directly to the I/O pins, which can be ualso used as digital pins if the ADC 
 * is disabled.
 * 
 * The ADC starts with a specific digital value, and then iterates various 
 * cycles, refining the digital value to get close to the corresponding 
 * analog input.
 * 
 * The atmega328p microcontroller contains just one ADC. However, there are many 
 * possible analog inputs, e.g. 6 external pins, internal temperature sensor,...
 * Because of that, the ADC input can be changed between the said "channels".
 * 
 * @todo Implement burst mode
 */

#ifndef __ANALOG
#define __ANALOG

#include <stdint.h>
#include <avr/io.h>


/**
 * @brief Prescaler values for the ADC
 * 
 * All the possible preescaler values for the ADC clock, as needed by the 
 * adc_init() function.
 * 
 * A single conversion takes 21 ADC cycles, while in burst mode, the first 
 * conversion takes 25 ADC cycles, and the following, 13.5 cycles.
 */
typedef enum {
	adc_presc_2 = 1,
	adc_presc_4 = 2,
	adc_presc_8 = 3,
	adc_presc_16 = 4,
	adc_presc_32 = 5,
	adc_presc_64 = 6,
	adc_presc_128 = 7
} adc_prescaler_t;


/**
 * @brief ADC reference voltage values
 * 
 * All the possible voltage values to use as reference by the ADC, as needed 
 * by the adc_init() function.
 */
typedef enum {
	adc_ref_aref = 0,       //!< Aref input pin
	adc_ref_vcc = 1,        //!< Vcc input pin
	adc_ref_internal = 3    //!< Internal reference of 1.1v
} adc_reference_t;


/**
 * @brief ADC channels
 * 
 * All the possible ADC input channels as needed by the adn_init() function.
 */
typedef enum {
	adc_channel_a0 = 0,
	adc_channel_a1 = 1,
	adc_channel_a2 = 2,
	adc_channel_a3 = 3,
	adc_channel_a4 = 4,
	adc_channel_a5 = 5,
	adc_channel_temperature = 8,    //!< Internal temperature sensor
	adc_channel_internal = 14,      //!< internal 1.1v input (Vbg)
	adc_channel_gnd = 15            //!< Ground
} adc_channel_t;


#define MSK_CH0 _BV(ADC0D)
#define MSK_CH1 _BV(ADC1D)
#define MSK_CH2 _BV(ADC2D)
#define MSK_CH3 _BV(ADC3D)
#define MSK_CH4 _BV(ADC4D)
#define MSK_CH5 _BV(ADC5D)


/**
 * @brief Initialize the analog-to-digital converter
 * 
 * This function must be called once before using any other ADC function.
 * A pinmask of the pins connected to analog inputs should be indicated for 
 * power eficiency.
 * 
 * @param prescaler Prescaler for the ADC clock
 * @param adc_reference Reference voltage for the ADC
 * @param channel Analog input channel
 * @param pinmask Mask of the pins connected to analog inputs
 * 
 */
void adc_init(adc_prescaler_t prescaler, adc_reference_t ref,
		adc_channel_t channel, uint8_t pinmask);
        
 
/**
 * @brief Change the adc channel
 * 
 * @param channel The new adc channel
 */
void adc_change_channel(adc_channel_t channel);


/**
 * @brief Performs a single analog read
 * 
 * The result voltage is calculated with the following formula
 * \f{eqnarray*}{
 *  V_{in} = \frac{ADC * V_{ref}}{1024}
 * \f}
 * Where \f$ ADC\f$ is the read value and \f$ V_{ref} \f$ is the reference voltage
 * 
 * @returns The digital representation of the analog input \f$ (0-1023) \f$
 */
uint8_t adc_single_read();

#endif /* __ANALOG */
