/**
 * @file stm32f4_ldr_sensor.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the LDR sensor port layer connected to the STM32F4 Nucleo board.
 * @version 0.1
 * @date 2025-05-01
 *
 */

#ifndef STM32F4_LDR_SENSOR_H_
#define STM32F4_LDR_SENSOR_H_

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define STM32F4_LDR_SENSOR_LAMP_GPIO_PORT        /*!< @hideinitializer GPIO port of the LDR sensor in the Nucleo board */
#define STM32F4_LDR_SENSOR_LAMP_GPIO_PIN         /*!< @hideinitializer GPIO pin of the LDR sensor in the Nucleo board */
#define STM32F4_LDR_SENSOR_LAMP_ADC              /*!< @hideinitializer ADC of the LDR sensor in the Nucleo board */
#define STM32F4_LDR_SENSOR_LAMP_ADC_CHANNEL      /*!< @hideinitializer ADC channel of the LDR sensor in the Nucleo board */
#define STM32F4_LDR_SENSOR_LAMP_ADC_IRQ ADC_IRQn /*!< ADC IRQn */
#define STM32F4_LDR_SENSOR_LAMP_ADC_IRQ_PRIO     /*!< ADC IRQn priority */
#define STM32F4_LDR_SENSOR_LAMP_ADC_IRQ_SUBPRIO  /*!< ADC IRQn subpriority */

/**
 * @brief Get the ADC handler of the LDR sensor.
 *
 * @param ldr_sensor_id LDR sensor ID.
 * @return ADC_HandleTypeDef* Pointer to the ADC handler of the LDR sensor.
 */
ADC_HandleTypeDef *stm32f4_get_ldr_sensor_adc_handler(uint32_t ldr_sensor_id);

#endif /* STM32F4_LDR_SENSOR_H_ */
