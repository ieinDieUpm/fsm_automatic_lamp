/**
 * @file main.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Basic FSM reading analog data from an LDR sensor and controlling a LED
 * @version 1.0
 * @date 2025-05-01
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

    // Create an automatic lamp FSM and get a pointer to it
    fsm_automatic_lamp_t *p_fsm_lamp = fsm_lamp_new(PORT_LED_LAMP_ID, PORT_LED_STATUS_ID, PORT_LDR_SENSOR_LAMP_ID);

    while (1)
    {
        // Launch the FSM
        fsm_lamp_fire(p_fsm_lamp);

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