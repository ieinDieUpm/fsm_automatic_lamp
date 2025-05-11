/**
 * @file stm32f4_automatic_lamp.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic lamp system port layer.
 * @version 0.1
 * @date 2025-05-01
 *
 */

#ifndef STM32F4_AUTOMATIC_LAMP_H
#define STM32F4_AUTOMATIC_LAMP_H

/* Includes ------------------------------------------------------------------*/
/* HW dependent includes */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER             /*!< Timer to measure the LDR */
#define STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ         /*!< Timer IRQn */
#define STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ_PRIO    /*!< Timer IRQn priority */
#define STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ_SUBPRIO /*!< Timer IRQn subpriority */
#define STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_PERIOD_MS   /*!< Timer period in milliseconds */

/**
 * @brief Get the timer handler of the automatic lamp.
 *
 * @param lamp_id Automatic lamp ID.
 * @return TIM_HandleTypeDef* Pointer to the timer handler of the automatic lamp.
 */
TIM_HandleTypeDef *stm32f4_get_automatic_lamp_timer_handler(uint32_t lamp_id);

#endif /* STM32F4_AUTOMATIC_LAMP_H */
