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

#ifndef HAL_ZEDF9P_H
#define HAL_ZEDF9P_H


#include "main.h"

#define ZEDF9P_TIMEOUT 1000
#define MAX_PAYLOAD_SIZE 202

int hal_init_zedf9p();
int hal_zedf9p_uart_receive(char* return_char);

struct rtcm_packet {
    void* fifo_reserved;
    char* message;   
    uint8_t length;
} typedef RTCM;




#endif