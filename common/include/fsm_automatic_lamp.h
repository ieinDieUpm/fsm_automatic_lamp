/**
 * @file fsm_automatic_lamp.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic lamp FSM.
 * @date 2025-05-01
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
#define AUTOMATIC_LAMP_HISTORY 10              /*!< Number of events to store in the automatic lamp */
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
typedef struct fsm_automatic_lamp_t fsm_automatic_lamp_t; /*!< Declaration of the FSM structure */

/* Function prototypes and explanations ---------------------------------------*/
/**
 * @brief Creates a new automatic lamp FSM.
 *
 * @param led_lamp_id ID of the lamp LED.
 * @param led_status_id ID of the status LED.
 * @param ldr_sensor_id ID of the LDR sensor.
 * @return fsm_automatic_lamp_t* Pointer to the new automatic lamp FSM.
 */
fsm_automatic_lamp_t *fsm_lamp_new(uint32_t led_lamp_id, uint32_t led_status_id, uint32_t ldr_sensor_id);

/**
 * @brief Gets the last time there was an event in the automatic lamp. If the event is not found, it returns 0.
 *
 * @param p_this Pointer to the automatic lamp FSM structure.
 * @param event Event to check. It can be any of the events in the LAMP_EVENTS enum.
 */
uint32_t fsm_lamp_get_last_time_event(fsm_automatic_lamp_t *p_this, uint8_t event);

/**
 * @brief Gets the automatic lamp status.
 *
 * @param p_this Pointer to the automatic lamp FSM structure.
 */
uint8_t fsm_lamp_get_status(fsm_automatic_lamp_t *p_this);

/**
 * @brief Fires the automatic lamp FSM.
 *
 * @param p_fsm Pointer to the automatic lamp FSM structure.
 */

void fsm_lamp_fire(fsm_automatic_lamp_t *p_fsm);

#endif /* FSM_AUTOMATIC_LAMP_H */