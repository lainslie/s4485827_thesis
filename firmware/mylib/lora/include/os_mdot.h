/**   
 ***************************************************************
 * @file    mylib/mdot/include/os_mdot.h    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/11/2021   
 * @brief   OS Lora Transmission header file    
 ***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* os_init_mdot
* queue_mdot_transmit_message
***************************************************************
 */

#ifndef OS_MDOT_H
#define OS_MDOT_H

#include "main.h"
#include "hal_mdot.h"
#include "hal_zedf9p.h"

/* FUNCTION PROTYPES */
int os_init_mdot();
void queue_mdot_transmit_message(RTCM *data);

#endif