/**
 * @file port_ldr_sensor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a LDR sensor.
 * @version 0.1
 * @date 2024-05-01
 *
 */

/* Standard C includes */
#include <stdint.h>
#include <stdio.h>

/* HW dependent includes */
#include "port_ldr_sensor.h"
#include "stm32f4xx.h"
#include "port_system.h"

/* Global variables -----------------------------------------------------------*/
port_ldr_hw_t ldr_sensor_lamp = {
    // TO-DO: complete the initialization of the LDR sensor
}; 

/* Private functions */

/**
 * @brief Converts an ADC value to millivolts.
 *
 * @param adcValue Counts of the ADC
 * @param adc_res_bits Resolution of the ADC in bits
 * @return uint32_t Millivolts
 */
uint32_t _adc_to_mvolts(uint32_t adcValue, uint8_t adc_res_bits)
{
    uint32_t mvolts = (ADC_VREF_MV * adcValue) / ((1 << adc_res_bits) - 1);
    return mvolts;
}
/* Function definitions ------------------------------------------------------*/
double port_ldr_sensor_get_resistance(port_ldr_hw_t *p_ldr)
{
    return p_ldr->resistance_ohms;
}

void port_ldr_sensor_save_adc_value(port_ldr_hw_t *p_ldr, double adc_value)
{
    // TO-DO: Convert the ADC value to light resistance in ohms looking at the voltage divider circuit
    

    // There are few problems to print double values using printf with SWO. The value is multiplied by 10 and printed as an integer the decimal point is added manually.
    printf("LDR: %ld ohms\n", p_ldr->resistance_ohms);
}

void port_ldr_sensor_init(port_ldr_hw_t *p_ldr)
{
    // Initialize the GPIO
    port_system_gpio_config(p_ldr->p_port, p_ldr->pin, GPIO_MODE_ANALOG, GPIO_PUPDR_NOPULL);

    // Initialize the ADC with 12-bit resolution and EOC interrupt enable
    port_system_adc_single_ch_init(p_ldr->p_adc, p_ldr->adc_channel, ADC_RESOLUTION_12B | ADC_EOC_INTERRUPT_ENABLE);

    // Enable the ADC global interrupt
    port_system_adc_interrupt_enable(1, 0);

    // Enable the ADC
    port_system_adc_enable(p_ldr->p_adc);
}