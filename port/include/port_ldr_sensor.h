/**
 * @file port_ldr_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the LDR sensor port layer.
 * @version 0.1
 * @date 2025-05-01
 *
 */

#ifndef PORT_LDR_SENSOR_H
#define PORT_LDR_SENSOR_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Defines and macros --------------------------------------------------------*/

#define PORT_LDR_SENSOR_LAMP_ID 0                /*!< LDR sensor ID for the lamp system. @hideinitializer */
#define PORT_LDR_SENSOR_LAMP_PULLDOWN_OHMS  /*!< @hideinitializer Pull-down resistor in Ohms of the LDR sensor */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Gets the LDR in Ohms of the LDR sensor.
 *
 * @param ldr_id LDR sensor ID.
 *
 * @return double LDR in Ohms.
 */
double port_ldr_sensor_get_resistance(uint32_t ldr_id);

/**
 * @brief Saves the ADC value of the LDR sensor and converts it to Ohms.
 *
 * @param ldr_id LDR sensor ID.
 * @param adc_value ADC value of the LDR sensor.
 */
void port_ldr_sensor_save_adc_value(uint32_t ldr_id, uint32_t adc_value);

/**
 * @brief Initializes the LDR sensor.
 *
 * @param ldr_id LDR sensor ID.
 */
void port_ldr_sensor_init(uint32_t ldr_id);

#endif /* PORT_LDR_SENSOR_H */