/**
 * @file stm32f4_ldr_sensor.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a LDR sensor.
 * @version 0.1
 * @date 2025-05-01
 *
 */

/* Standard C includes */
#include <stdint.h>
#include <stdio.h>

/* HW dependent includes */
#include "port_system.h"
#include "port_ldr_sensor.h"

/* Microcontroller dependent includes */
#include "stm32f4_ldr_sensor.h"

/* Defines and macros --------------------------------------------------------*/
#define RESOLUTION_12_BIT 12 /*!< ADC resolution in bits */

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of a LDR sensor.
 */
typedef struct
{
    GPIO_TypeDef *p_port;         /*!< GPIO where the LDR is connected */
    uint8_t pin;                  /*!< Pin/line where the LDR is connected */
    ADC_TypeDef *p_adc;           /*!< ADC where the LDR is connected */
    uint32_t adc_channel;         /*!< ADC channel where the LDR is connected */
    IRQn_Type adc_irq;            /*!< ADC IRQn */
    uint8_t adc_irq_prio;         /*!< ADC IRQn priority */
    uint8_t adc_irq_subprio;      /*!< ADC IRQn subpriority */
    ADC_HandleTypeDef adc_handle; /*!< ADC handle */
    uint32_t resistance_ohms;     /*!< Resistance of the LDR in Ohms */
} stm32f4_ldr_hw_t;

/* Global variables -----------------------------------------------------------*/
static stm32f4_ldr_hw_t ldr_sensor_lamp_arr[] = {
    [PORT_LDR_SENSOR_LAMP_ID] = {

    }
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
    uint32_t mvolts = (VDD_VALUE * adcValue) / ((1 << adc_res_bits) - 1);
    return mvolts;
}

/* Function definitions ------------------------------------------------------*/
double port_ldr_sensor_get_resistance(uint32_t ldr_id)
{
    // Get the LDR structure from the array
    stm32f4_ldr_hw_t *p_ldr = &ldr_sensor_lamp_arr[ldr_id];

    double resistance_ohms = p_ldr->resistance_ohms;
    return resistance_ohms;
}

void port_ldr_sensor_save_adc_value(uint32_t ldr_id, uint32_t adc_value)
{
    // Get the LDR structure from the array
    stm32f4_ldr_hw_t *p_ldr = &ldr_sensor_lamp_arr[ldr_id];

    // Convert the ADC value to light resistance in ohms looking at the voltage divider circuit

    
    printf("LDR: %ld ohms\n", p_ldr->resistance_ohms);
}

void port_ldr_sensor_init(uint32_t ldr_id)
{
    // Get the LDR structure from the array
    stm32f4_ldr_hw_t *p_ldr = &ldr_sensor_lamp_arr[ldr_id];

    // Get the GPIO port and pin
    GPIO_TypeDef *p_port = p_ldr->p_port;
    uint16_t pin = p_ldr->pin;

    // Initialize the GPIO port and pin
    // Create a generic LDR structure to configure the GPIO
    GPIO_InitTypeDef ldr_gpio = {0}; // Initialize to 0 all fields by default

    /* GPIO Ports Clock Enable */
    if (p_port == GPIOA)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if (p_port == GPIOB)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if (p_port == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else
    {
        // Error: Invalid GPIO port
        return;
    }
    /* Configure GPIO pin */
    ldr_gpio.Pin = pin;
    ldr_gpio.Mode = GPIO_MODE_ANALOG; // A LDR is an analog input device
    ldr_gpio.Pull = GPIO_NOPULL;      // No pull-up or pull-down internal resistors
    HAL_GPIO_Init(p_port, &ldr_gpio); // Initialize the GPIO pin

    // Get the ADC instance
    ADC_TypeDef *p_adc = p_ldr->p_adc;

    // Initialize the ADC handle
    ADC_HandleTypeDef *adc_handle = &p_ldr->adc_handle;

    // Habilitate the ADC clock
    if (p_adc == ADC1)
    {
        __HAL_RCC_ADC1_CLK_ENABLE();
    }
    else if (p_adc == ADC2)
    {
        __HAL_RCC_ADC2_CLK_ENABLE();
    }
    else if (p_adc == ADC3)
    {
        __HAL_RCC_ADC3_CLK_ENABLE();
    }
    else
    {
        // Error: Invalid ADC instance
        return;
    }

    // Assign the ADC instance to the handle
    adc_handle->Instance = p_adc;

    // Disable the ADC before initialization
    __HAL_ADC_DISABLE(adc_handle);

    // Configure the ADC
    adc_handle->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // Set the ADC clock prescaler

    // Estas configuraciones se pueden quitar, son las que vienen por defecto
    adc_handle->Init.Resolution = ADC_RESOLUTION_12B;
    adc_handle->Init.ScanConvMode = DISABLE;
    adc_handle->Init.ContinuousConvMode = DISABLE;
    adc_handle->Init.DiscontinuousConvMode = DISABLE;
    adc_handle->Init.NbrOfDiscConversion = 0;
    adc_handle->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_handle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle->Init.NbrOfConversion = 1;
    adc_handle->Init.DMAContinuousRequests = DISABLE;
    adc_handle->Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    // Inicializar el ADC
    HAL_ADC_Init(adc_handle);

    // Configure the ADC channel
    ADC_ChannelConfTypeDef ldr_channel = {0};
    ldr_channel.Channel = p_ldr->adc_channel;          // Set the ADC channel
    ldr_channel.Rank = 1;                              // Set the rank to 1
    ldr_channel.SamplingTime = ADC_SAMPLETIME_3CYCLES; // Set the sampling time

    // Initialize the ADC channel
    HAL_ADC_ConfigChannel(adc_handle, &ldr_channel);

    // Enable the ADC interrupt
    HAL_NVIC_SetPriority(p_ldr->adc_irq, p_ldr->adc_irq_prio, p_ldr->adc_irq_subprio);
    HAL_NVIC_EnableIRQ(p_ldr->adc_irq);
}

void port_ldr_sensor_start_conversion(uint32_t ldr_id)
{
    // Get the temperature structure from the array
    stm32f4_ldr_hw_t *p_ldr = &ldr_sensor_lamp_arr[ldr_id];

    // Start the ADC conversion
    HAL_ADC_Start_IT(&p_ldr->adc_handle);
}

ADC_HandleTypeDef *stm32f4_get_ldr_sensor_adc_handler(uint32_t ldr_id)
{
    // Get the temperature structure from the array
    stm32f4_ldr_hw_t *p_ldr = &ldr_sensor_lamp_arr[ldr_id];

    // Return the ADC handle
    return &p_ldr->adc_handle;
}