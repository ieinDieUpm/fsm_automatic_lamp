/**
 * @file port_ldr_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the LDR sensor port layer.
 * @version 0.1
 * @date 2024-05-01
 *
 */

#ifndef PORT_LDR_SENSOR_H
#define PORT_LDR_SENSOR_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>

/* HW dependent includes */
#include "port_system.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define LDR_SENSOR_LAMP_GPIO        /*!< GPIO port of the LDR sensor in the Nucleo board */
#define LDR_SENSOR_LAMP_PIN         /*!< GPIO pin of the LDR sensor in the Nucleo board */
#define LDR_SENSOR_LAMP_ADC         /*!< ADC of the LDR sensor in the Nucleo board */
#define LDR_SENSOR_LAMP_ADC_CHANNEL /*!< ADC channel of the LDR sensor in the Nucleo board */
#define R_PULLDOWN_OHMS             /*!< Pull-down resistor in Ohms of the LDR sensor */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a LDR sensor.
 */
typedef struct
{
    GPIO_TypeDef *p_port;     /*!< GPIO where the LDR is connected */
    uint8_t pin;              /*!< Pin/line where the LDR is connected */
    ADC_TypeDef *p_adc;       /*!< ADC where the LDR is connected */
    uint32_t adc_channel;     /*!< ADC channel where the LDR is connected */
    uint32_t resistance_ohms; /*!< Resistance of the LDR in Ohms */
} port_ldr_hw_t;

/* Global variables -----------------------------------------------------------*/
extern port_ldr_hw_t ldr_sensor_lamp; /*!< LDR sensor of the automatic lamp system. Public for access to interrupt handlers. */

/**
 * @brief Gets the LDR in Ohms of the LDR sensor.
 *
 * @return double LDR in Ohms.
 */
double port_ldr_sensor_get_resistance(port_ldr_hw_t *pir_sensor);

/**
 * @brief Saves the ADC value of the LDR sensor and converts it to Ohms.
 *
 * @param p_ldr Pointer to the LDR sensor structure.
 * @param adc_value ADC value of the LDR sensor.
 */
void port_ldr_sensor_save_adc_value(port_ldr_hw_t *p_ldr, double adc_value);

/**
 * @brief Initializes the LDR sensor.
 *
 * @param pir_sensor Pointer to the LDR sensor structure.
 */
void port_ldr_sensor_init(port_ldr_hw_t *pir_sensor);

#endif /* PORT_LDR_SENSOR_H */