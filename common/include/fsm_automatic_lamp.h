/**
 * @file fsm_automatic_lamp.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic lamp FSM.
 * @date 2024-05-01
 *
 */

#ifndef FSM_AUTOMATIC_LAMP_H
#define FSM_AUTOMATIC_LAMP_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Other includes */
#include <fsm.h>
#include "port_led.h"
#include "port_ldr_sensor.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define AUTOMATIC_LAMP_SAMPLING_PERIOD_SEC 1  /*!< Sampling period for the automatic lamp */
#define AUTOMATIC_LAMP_HISTORY 10             /*!< Number of events to store in the automatic lamp */
#define AUTOMATIC_LAMP_DEFAULT_THRESHOLD 50000 /*!< Threshold in Ohms to activate the automatic lamp */

/* Enums */
/**
 * @brief Enumerates the states of the automatic lamp FSM.
 *
 */
enum FSM_AUTOMATIC_LAMP_STATES
{
    LAMP_OFF = 0, /*!< Lamp is off */
    LAMP_ON       /*!< Lamp is on */
};

/**
 * @brief Enumerates the events of the automatic lamp FSM.
 *
 */
enum LAMP_EVENTS
{
    UNKNOWN = -1,   /*!< Unknown event */
    ACTIVATION = 0, /*!< Lamp activated */
    DEACTIVATION    /*!< Lamp deactivated */
};

/* Typedefs ------------------------------------------------------------------*/
/**
 * @brief Structure to define the automatic lamp FSM.
 */
typedef struct
{
    fsm_t f;                                           /*!< FSM structure. Important to be the first element of the structure */
    port_led_hw_t *p_led_lamp;                         /*!< Pointer to the LED structure */
    port_led_hw_t *p_led_status;                       /*!< Pointer to the status LED structure */
    port_ldr_hw_t *p_ldr_sensor;                      /*!< Pointer to the LDR sensor structure */
    bool last_events[AUTOMATIC_LAMP_HISTORY];          /*!< Statuses of the automatic lamp */
    uint32_t last_time_events[AUTOMATIC_LAMP_HISTORY]; /*!< Last times of events detected */
    uint8_t event_idx;                                 /*!< Index of the last event */
    double threshold_ldr_ohms;                         /*!< Threshold LDR to activate the automatic lamp Ohms */
    uint32_t timer_period_sec;                         /*!< Period of the timer to measure the LDR */
} fsm_automatic_lamp_t;

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Creates a new automatic lamp FSM.
 *
 * @param p_led_lamp Pointer to the LED of the automatic lamp.
 * @param p_led_status Pointer to the status LED of the automatic lamp.
 * @param p_ldr Pointer to the LDR sensor of the automatic lamp.
 * @return fsm_automatic_lamp_t* Pointer to the new automatic lamp FSM.
 */
fsm_t *fsm_lamp_new(port_led_hw_t *p_led_lamp, port_led_hw_t *p_led_status, port_ldr_hw_t *p_ldr);

/**
 * @brief Gets the last time there was an event in the automatic lamp. If the event is not found, it returns 0.
 *
 * @param p_this Pointer to the automatic lamp FSM structure.
 * @param event Event to check. It can be any of the events in the LAMP_EVENTS enum.
 */
uint32_t fsm_lamp_get_last_time_event(fsm_t *p_this, uint8_t event);

/**
 * @brief Gets the automatic lamp status.
 *
 * @param p_this Pointer to the automatic lamp FSM structure.
 */
uint8_t fsm_lamp_get_status(fsm_t *p_this);

#endif /* FSM_AUTOMATIC_LAMP_H */