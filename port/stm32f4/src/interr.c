/**
 * @file interr.c
 * @brief Interrupt service routines for the STM32F4 platform.
 * @author Josué Pagán (j.pagan@upm.es)
 * @date 2024-04-01
 */
// Include headers of different port elements:
#include "stm32f4xx.h"
#include "port_system.h"
#include "port_led.h"
#include "port_ldr_sensor.h"

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
  // Identify if the ADC that generated the interrupt is the same as the LDR sensor
  if (ldr_sensor_lamp.p_adc->SR & ADC_SR_EOC)
  {
    // TO-DO: Read the LDR sensor value
    
    
    // Clear the ADC interrupt flag
    ldr_sensor_lamp.p_adc->SR &= ~ADC_SR_EOC;
  }
}

// TO-DO: Add the interrupt service routine for the timer of the automatic lamp

