/**
 * @file stm32f4_automatic_lamp.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Source file for the automatic lamp system port layer.
 * @version 1.0
 * @date 2025-05-01
 *
 */

/* Standard C includes */
#include <stdlib.h>
#include <math.h>

/* Project includes */
#include "port_automatic_lamp.h"

/* Microcontroller dependent includes */
#include "stm32f4_automatic_lamp.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an automatic lamp.
 *
 */
typedef struct
{
    TIM_TypeDef *p_timer;                         /*!< Timer to control the lamp */
    IRQn_Type timer_irq;                          /*!< Timer IRQn */
    uint8_t timer_irq_prio;                       /*!< Timer IRQn priority */
    uint8_t timer_irq_subprio;                    /*!< Timer IRQn subpriority */
    uint32_t timer_period;                        /*!< Timer period in seconds */
    TIM_HandleTypeDef handler_tim_automatic_lamp; /*!< Timer handler */
} stm32f4_automatic_lamp_hw_t;

/* Global variables -----------------------------------------------------------*/
/**
 * @brief Array of automatic lamp structures to define the HW dependencies of the automatic lamps.
 *
 */
static stm32f4_automatic_lamp_hw_t automatic_lamps_arr[] = {
    [PORT_AUTOMATIC_LAMP_ID] = {
        .p_timer = STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER,
        .timer_irq = STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ,
        .timer_irq_prio = STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ_PRIO,
        .timer_irq_subprio = STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_IRQ_SUBPRIO,
        .timer_period = STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER_PERIOD_MS,
    },
};

/* Private functions ---------------------------------------------------------*/
void port_automatic_lamp_timer_setup(uint32_t lamp_id)
{
    // Get the automatic lamp structure from the array
    stm32f4_automatic_lamp_hw_t *p_automatic_lamp = &automatic_lamps_arr[lamp_id];

    // Get the timer instance
    TIM_TypeDef *p_timer = p_automatic_lamp->p_timer;

    // Get the timer handler
    TIM_HandleTypeDef *p_handler_timer = &p_automatic_lamp->handler_tim_automatic_lamp;

    // Enable the timer clock using HAL macro
    if (p_timer == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
    else if (p_timer == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
    else
    {
        // Invalid timer
        return;
    }

    // Assign the timer instance to the handler
    p_handler_timer->Instance = p_timer;

    // Stop timer with interrupt enabled
    HAL_TIM_Base_Stop_IT(p_handler_timer);

    // Compute ARR and PSC to match the duration in seconds. Check if the duration is too long and adapt prescaler and ARR
    double ms = (double)p_automatic_lamp->timer_period;
    double scc = HSI_VALUE; // System clock frequency in Hz
    double psc = round((((scc / 1000.0) * ms) / (65535.0 + 1.0)) - 1.0);
    double arr = round((((scc / 1000.0) * ms) / (psc + 1.0)) - 1.0);

    // Adjust psc and arr if necessary
    while (arr > 0xFFFF)
    {
        psc += 1.0;
        arr = round(((scc / 1000.0) * ms) / (psc + 1.0) - 1.0);
    }

    // Set the timer parameters
    // Set the prescaler and auto-reload register
    p_handler_timer->Init.Prescaler = (uint32_t)(round(psc));
    p_handler_timer->Init.Period = (uint32_t)(round(arr));

    // Enable the auto-reload preload
    p_handler_timer->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    // Initialize the timer with the new PSC and ARR values
    HAL_TIM_Base_Init(p_handler_timer);

    // Reset counter value
    __HAL_TIM_SET_COUNTER(p_handler_timer, 0);

    // Clear update flag (IMPORTANT to avoid false interrupt)
    __HAL_TIM_CLEAR_FLAG(p_handler_timer, TIM_FLAG_UPDATE);

    // Set priority and enable IRQ in NVIC
    HAL_NVIC_SetPriority(p_automatic_lamp->timer_irq, p_automatic_lamp->timer_irq_prio, p_automatic_lamp->timer_irq_subprio);

    // Enable the timer
    HAL_TIM_Base_Start_IT(p_handler_timer);

    // Enable the timer interrupt
    HAL_NVIC_EnableIRQ(p_automatic_lamp->timer_irq);
}



TIM_HandleTypeDef *stm32f4_get_automatic_lamp_timer_handler(uint32_t lamp_id)
{
    // Get the automatic lamp structure from the array
    stm32f4_automatic_lamp_hw_t *p_automatic_lamp = &automatic_lamps_arr[lamp_id];

    // Return the timer handler
    return &p_automatic_lamp->handler_tim_automatic_lamp;
}