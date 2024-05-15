/**
 * @file main.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Basic FSM reading analog data from an LDR sensor and controlling a LED
 * @version 0.1
 * @date 2024-05-01
 *
 */

/* INCLUDES */
#include <stdio.h>
#include "port_system.h"
#include "port_led.h"
#include "fsm_automatic_lamp.h"

/* Defines and macros --------------------------------------------------------*/

/* MAIN FUNCTION */
#define DISABLE_SWO

/**
 * @brief Main function
 *
 * @return int
 */
int main()
{
    // Local variables
    uint8_t previous_lamp_status = UNKNOWN;

    /* Init board */
    port_system_init();

#ifdef DISABLE_SWO
    // Initialize the GPIOs for the LED on which might be off and it is not part of the FSM
    port_system_gpio_config(GPIOB, 3, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);    
    GPIOB->ODR &= ~BIT_POS_TO_MASK(3);
#endif

    // Create an automatic lamp FSM and get a pointer to it
    fsm_t *p_fsm_lamp = fsm_lamp_new(&led_lamp, &led_status, &ldr_sensor_lamp);

    while (1)
    {
        // Launch the FSM
        fsm_fire(p_fsm_lamp);

        uint8_t current_lamp_status = fsm_lamp_get_status(p_fsm_lamp);
        if (current_lamp_status != previous_lamp_status)
        {
            uint32_t last_time_activated = fsm_lamp_get_last_time_event(p_fsm_lamp, current_lamp_status);
            if (current_lamp_status == ACTIVATION)
            {
                printf("Lamp ON at %ld\n", last_time_activated);
            }
            else if (current_lamp_status == DEACTIVATION)
            {
                printf("Lamp OFF at %ld\n", last_time_activated);
            }
            previous_lamp_status = current_lamp_status;
        }
    }
    return 0;
}