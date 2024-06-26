/**
 * @file port_led.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Port layer for a LED.
 * @date 01-03-2024
 */
/* Standard C includes */
#include <stdio.h>
#include <math.h>

/* HW dependent includes */
#include "stm32f4xx.h"
#include "port_led.h"
#include "port_system.h"

/* Global variables -----------------------------------------------------------*/
port_led_hw_t led_lamp = {.p_port = LED_LAMP_GPIO, .pin = LED_LAMP_PIN};
port_led_hw_t led_status = {.p_port = LED_STATUS_GPIO, .pin = LED_STATUS_PIN};

bool port_led_get_status(port_led_hw_t *p_led)
{
    return (p_led->p_port->IDR & BIT_POS_TO_MASK(p_led->pin)) != 0;
}

void port_led_on(port_led_hw_t *p_led)
{
    p_led->p_port->ODR |= BIT_POS_TO_MASK(p_led->pin);
}

void port_led_off(port_led_hw_t *p_led)
{
    p_led->p_port->ODR &= ~BIT_POS_TO_MASK(p_led->pin);
}

void port_led_toggle(port_led_hw_t *p_led)
{
    p_led->p_port->ODR ^= BIT_POS_TO_MASK(p_led->pin);
}

void port_led_init(port_led_hw_t *p_led)
{
    port_system_gpio_config(p_led->p_port, p_led->pin, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);    
}