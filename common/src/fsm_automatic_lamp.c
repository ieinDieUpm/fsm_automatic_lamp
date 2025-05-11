/**
 * @file fsm_automatic_lamp.c
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Finite State Machine for an automatic lamp.
 * @date 2025-05-01
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
#include "port_system.h"

/**
 * @brief Structure to define the automatic lamp FSM.
 */
struct fsm_automatic_lamp_t
{
    fsm_t f;                                           /*!< FSM structure. Important to be the first element of the structure */
    uint32_t led_lamp_id;                              /*!< ID of the lamp LED */
    uint32_t led_status_id;                            /*!< ID of the status LED */
    uint32_t ldr_sensor_id;                            /*!< ID of the LDR sensor */
    bool last_events[AUTOMATIC_LAMP_HISTORY];          /*!< Statuses of the automatic lamp */
    uint32_t last_time_events[AUTOMATIC_LAMP_HISTORY]; /*!< Last times of events detected */
    uint8_t event_idx;                                 /*!< Index of the last event */
    double threshold_ldr_ohms;                         /*!< Threshold LDR to activate the automatic lamp Ohms */
};

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

    // Get the LDR resistance from the sensor


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
    port_led_set_value(p_fsm->led_lamp_id, true);
    port_led_set_value(p_fsm->led_status_id, false);

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
    port_led_set_value(p_fsm->led_lamp_id, false);
    port_led_set_value(p_fsm->led_status_id, true);

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

uint32_t fsm_lamp_get_last_time_event(fsm_automatic_lamp_t *p_fsm, uint8_t event)
{
    // Return the last time the event was detected
    if (p_fsm->event_idx == 0)
    {
        return p_fsm->last_time_events[AUTOMATIC_LAMP_HISTORY - 1];
    }
    else
    {
        return p_fsm->last_time_events[p_fsm->event_idx - 1];
    }
}

uint8_t fsm_lamp_get_status(fsm_automatic_lamp_t *p_fsm)
{
    // Return the last event detected
    if (p_fsm->event_idx == 0)
    {
        return p_fsm->last_events[AUTOMATIC_LAMP_HISTORY - 1];
    }
    else
    {
        return p_fsm->last_events[p_fsm->event_idx - 1];
    }
}

/* Initialize the FSM */

/**
 * @brief Initialize the automatic lamp FSM
 *
 * @param p_fsm_automatic_lamp Pointer to the automatic lamp FSM structure
 * @param led_lamp_id ID of the lamp LED
 * @param led_status_id ID of the status LED
 * @param ldr_sensor_id ID of the LDR sensor
 */
void fsm_automatic_lamp_init(fsm_automatic_lamp_t *p_fsm_automatic_lamp, uint32_t led_lamp_id, uint32_t led_status_id, uint32_t ldr_sensor_id)
{
    fsm_automatic_lamp_t *p_fsm = (fsm_automatic_lamp_t *)(p_fsm_automatic_lamp);
    fsm_init(&p_fsm_automatic_lamp->f, fsm_trans_automatic_lamp);

    // Assign the peripherals to the FSM
    p_fsm->led_lamp_id = led_lamp_id;
    p_fsm->led_status_id = led_status_id;
    p_fsm->ldr_sensor_id = ldr_sensor_id;

    // Initialize the last time the automatic lamp was activated
    memset(p_fsm->last_time_events, 0, sizeof(p_fsm->last_time_events));

    // Initialize the automatic lamp status
    memset(p_fsm->last_events, UNKNOWN, sizeof(p_fsm->last_events));

    // Initialize the event index
    p_fsm->event_idx = 0;

    // Initialize the threshold LDR
    p_fsm->threshold_ldr_ohms = AUTOMATIC_LAMP_DEFAULT_THRESHOLD;

    // Initialize the timer
    port_automatic_lamp_timer_setup(p_fsm->led_lamp_id);

    // Initialize the peripherals
    port_led_init(led_lamp_id);
    port_led_init(led_status_id);
    port_ldr_sensor_init(ldr_sensor_id);
}

/* Create FSM */
fsm_automatic_lamp_t *fsm_lamp_new(uint32_t led_lamp_id, uint32_t led_status_id, uint32_t ldr_sensor_id)
{
    // Create a new FSM structure
    fsm_automatic_lamp_t *p_fsm_automatic_lamp = malloc(sizeof(fsm_automatic_lamp_t));

    // Initialize the FSM
    fsm_automatic_lamp_init(p_fsm_automatic_lamp, led_lamp_id, led_status_id, ldr_sensor_id);

    return p_fsm_automatic_lamp;
}

void fsm_lamp_fire(fsm_automatic_lamp_t *p_fsm)
{
    fsm_fire((fsm_t *)p_fsm); // Call the FSM fire function
}