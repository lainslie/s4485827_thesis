/**   
 ***************************************************************
 * @file    mylib/hci/include/ahu_cli.h    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/03/2021   
 * @brief   AHU cli header file    
 ***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* cli_ahu_init 
***************************************************************
 */

#ifndef OS_MDOT_H
#define OS_MDOT_H


#include "main.h"
#include "hal_mdot.h"
#include "hal_zedf9p.h"

int os_init_mdot();
void queue_mdot_transmit_message(RTCM *data);



#endif