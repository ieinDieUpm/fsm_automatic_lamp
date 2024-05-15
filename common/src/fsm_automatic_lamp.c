/**
 * @file fsm_automatic_lamp.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Finite State Machine for an automatic lamp.
 * @date 2024-05-01
 *
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <string.h>

/* Project includes */
#include "fsm_automatic_lamp.h"
#include "port_automatic_lamp.h"
#include "port_led.h"
#include "port_ldr_sensor.h"

/* State machine input or transition functions */

/**
 * @brief Check if the value of impedance of the LDR is low enough to activate the automatic lamp
 *
 * @param p_this Pointer to the FSM structure
 * @return true if the impedance of the LDR is low, false otherwise
 */
bool check_on(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LDR sensor
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)p_this;

    // TODO: Implement the check_on function
    
}

/**
 * @brief Check if the value of impedance of the LDR is high enough to deactivate the automatic lamp
 *
 * @param p_this Pointer to the FSM structure
 * @return true if the impedance of the LDR is high, false otherwise
 */
bool check_off(fsm_t *p_this)
{
    return !check_on(p_this);
}

/* State machine output or action functions */

/**
 * @brief Turn off the automatic lamp
 *
 * @param p_this Pointer to the automatic lamp FSM structure
 */
void do_lamp_on(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)p_this;

    // Set the LEDs according to the automatic lamp status
    port_led_on(p_fsm->p_led_lamp);
    port_led_off(p_fsm->p_led_status);

    // Store the event
    p_fsm->last_events[p_fsm->event_idx] = ACTIVATION;
    p_fsm->last_time_events[p_fsm->event_idx] = port_system_get_millis();
    p_fsm->event_idx = (p_fsm->event_idx + 1) % AUTOMATIC_LAMP_HISTORY;
}

/**
 * @brief Turn on the automatic lamp
 *
 * @param p_this Pointer to the automatic lamp FSM structure
 */
void do_lamp_off(fsm_t *p_this)
{
    // Retrieve the FSM structure and get the LED
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)p_this;

    // Set the LEDs according to the automatic lamp status
    port_led_off(p_fsm->p_led_lamp);
    port_led_on(p_fsm->p_led_status);

    // Store the event
    p_fsm->last_events[p_fsm->event_idx] = DEACTIVATION;
    p_fsm->last_time_events[p_fsm->event_idx] = port_system_get_millis();
    p_fsm->event_idx = (p_fsm->event_idx + 1) % AUTOMATIC_LAMP_HISTORY;
}

/* Transitions table ---------------------------------------------------------*/
/**
 * @brief Transitions table for the automatic lamp
 *
 */
fsm_trans_t fsm_trans_automatic_lamp[] = {
    {LAMP_OFF, check_on, LAMP_ON, do_lamp_on},
    {LAMP_ON, check_off, LAMP_OFF, do_lamp_off},
    {-1, NULL, -1, NULL},
};

uint32_t fsm_lamp_get_last_time_event(fsm_t *p_this, uint8_t event)
{
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)p_this;

    // Look for the last time the event was detected, if it was detected. If not, return 0
    for (int i = 0; i < AUTOMATIC_LAMP_HISTORY; i++)
    {
        if (p_fsm->last_events[i] == event)
        {
            return p_fsm->last_time_events[i];
        }
    }
    return 0;
}

uint8_t fsm_lamp_get_status(fsm_t *p_this)
{
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)p_this;
    return p_fsm->last_events[p_fsm->event_idx];
}

/* Initialize the FSM */

/**
 * @brief Initialize the automatic lamp FSM
 *
 * @param p_this Pointer to the FSM structure
 * @param p_led_lamp Pointer to the LED structure
 * @param p_led_status Pointer to the LED structure
 * @param p_ldr Pointer to the LDR sensor structure
 */
void fsm_automatic_lamp_init(fsm_t *p_this, port_led_hw_t *p_led_lamp, port_led_hw_t *p_led_status, port_ldr_hw_t *p_ldr)
{
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)(p_this);
    fsm_init(p_this, fsm_trans_automatic_lamp);

    // Assign the peripherals to the FSM
    p_fsm->p_led_lamp = p_led_lamp;
    p_fsm->p_led_status = p_led_status;
    p_fsm->p_ldr_sensor = p_ldr;

    // Initialize the last time the automatic lamp was activated
    memset(p_fsm->last_time_events, 0, sizeof(p_fsm->last_time_events));

    // Initialize the automatic lamp status
    memset(p_fsm->last_events, UNKNOWN, sizeof(p_fsm->last_events));

    // Initialize the event index
    p_fsm->event_idx = 0;

    // Initialize the threshold LDR
    p_fsm->threshold_ldr_ohms = AUTOMATIC_LAMP_DEFAULT_THRESHOLD;

    // Initialize the timer to measure the LDR
    p_fsm->timer_period_sec = AUTOMATIC_LAMP_SAMPLING_PERIOD_SEC;

    // Initialize the timer
    port_automatic_lamp_timer_setup(p_fsm);

    // Initialize the peripherals
    port_led_init(p_led_lamp);
    port_led_init(p_led_status);    
    port_ldr_sensor_init(p_ldr);
}

/* Create FSM */
fsm_t *fsm_lamp_new(port_led_hw_t *p_led_lamp, port_led_hw_t *p_led_status, port_ldr_hw_t *p_ldr)
{
    // Do malloc for the whole FSM structure to reserve memory for the rest of the FSM, although I interpret it as fsm_t which is the first field of the structure so that the FSM library can work with it
    fsm_t *p_fsm = malloc(sizeof(fsm_automatic_lamp_t));

    // Initialize the FSM
    fsm_automatic_lamp_init(p_fsm, p_led_lamp, p_led_status, p_ldr);

    return p_fsm;
}
