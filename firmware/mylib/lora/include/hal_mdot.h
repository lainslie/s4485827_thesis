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

#ifndef HAL_MDOT_H
#define HAL_MDOT_H


#include "main.h"
#include "hal_zedf9p.h"

#define AT_BASE_LEN 4
#define MDOT_TIMEOUT 10000
#define NETWORK_KEY "7BD1117E510F3EB7EF1451A95D073823"
#define NETWORK_ID "0000000000000000"
#define DEV_ID "70B3D57ED0046710"


int hal_init_mdot();
int hal_mdot_send_at(char* command, int timeout);
int hal_mdot_send_payload(RTCM* payload);




#endif