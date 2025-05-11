/**
 * @file port_automatic_lamp.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic lamp system port layer.
 * @version 0.1
 * @date 2025-05-01
 *
 */

#ifndef PORT_AUTOMATIC_LAMP_H
#define PORT_AUTOMATIC_LAMP_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Defines and macros --------------------------------------------------------*/
#define PORT_AUTOMATIC_LAMP_ID 0 /*!< Thermostat identifier @hideinitializer */

/**
 * @brief Initializes the timer of the automatic lamp.
 *
 * @param automatic_lamp_id Automatic lamp ID.
 */
void port_automatic_lamp_timer_setup(uint32_t automatic_lamp_id);

#endif /* PORT_AUTOMATIC_LAMP_H */
