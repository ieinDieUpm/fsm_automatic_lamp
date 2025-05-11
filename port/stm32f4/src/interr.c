/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author Josué Pagán (j.pagan@upm.es)
 * @date 2025-04-01
 */
// C Standard includes
#include <stdio.h>

// Include HW dependencies:
#include "port_system.h"

// Include headers of different port elements:
#include "port_ldr_sensor.h"
#include "stm32f4_ldr_sensor.h"
#include "port_automatic_lamp.h"
#include "stm32f4_automatic_lamp.h"

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief Interrupt service routine for the System tick timer (SysTick).
 *
 * @note This ISR is called when the SysTick timer generates an interrupt.
 * The program flow jumps to this ISR and increments the tick counter by one millisecond.
 *
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified by a call of an ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition)
. **Added to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
  port_system_set_millis(port_system_get_millis() + 1);
}


/**
 * @brief Interrupt service routine for all the ADCs.
 *
 * @note This ISR is called when any ADC generates an interrupt.
 *
 */
void ADC_IRQHandler(void)
{
  // Get the ADC handle
  ADC_HandleTypeDef *p_handler_adc = stm32f4_get_ldr_sensor_adc_handler(PORT_AUTOMATIC_LAMP_ID);

  // Call the HAL ADC IRQ handler to clear the interrupt flag and handle the conversion with the callback function
  HAL_ADC_IRQHandler(p_handler_adc);
}

/**
 * @brief  Timer period elapsed callback in non blocking mode
 * @param  handler_tim: TIM handle
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handler_tim)
{
  if (handler_tim->Instance == STM32F4_AUTOMATIC_LAMP_MEASUREMENT_TIMER) // If the timer is the automatic lamp measurement timer
  {
    // Get the ADC handle of the LDR sensor
    

    // Start the ADC conversion
    

  }
}


/**
 * @brief ADC conversion complete callback.
 *
 * @param p_handler_adc Pointer to the ADC handle.
 *
 * @note This function is called when the ADC conversion is complete.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *p_handler_adc)
{
  if (p_handler_adc->Instance == STM32F4_LDR_SENSOR_LAMP_ADC)
  {
    // Read the ADC value


    // Print the ADC value
    

    // Call the function to save the ADC value
    
    
  }
}