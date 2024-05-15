/**
 * @file port_automatic_lamp.h
 * @author Josué Pagán (j.pagan@upm.es)
 * @brief Header file for the automatic lamp system port layer.
 * @version 0.1
 * @date 2024-05-01
 *
 */

#ifndef PORT_AUTOMATIC_LAMP_H
#define PORT_AUTOMATIC_LAMP_H

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */

/* HW dependent includes */
#include "port_system.h"
#include "fsm_automatic_lamp.h"

/* Defines and macros --------------------------------------------------------*/
// HW Nucleo-STM32F446RE:
#define AUTOMATIC_LAMP_MEASUREMENT_TIMER /*!< Timer to measure the LDR */

/**
 * @brief Initializes the timer of the automatic lamp.
 *
 * @param p_automatic_lamp Pointer to the automatic lamp structure.
 */
void port_automatic_lamp_timer_setup(fsm_automatic_lamp_t *p_automatic_lamp);

#endif
