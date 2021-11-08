/**   
 ***************************************************************
 * @file    mylib/gps/include/hal_zedf9p.h    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/11/21   
 * @brief   HAL ZED-F9P header file    
 ***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
*  hal_init_zedf9p
*  hal_zedf9p_uart_receive
***************************************************************
 */

#ifndef HAL_ZEDF9P_H
#define HAL_ZEDF9P_H

#include "main.h"

/* MACRO DEFINED, CAN BE ALTERENED BY USER */
#define ZEDF9P_TIMEOUT 1000
#define MAX_PAYLOAD_SIZE 120
#define ZED_UART_DRIVER "UART_1"

/* FUNCTION PROTOTYPES */
int hal_init_zedf9p();
int hal_zedf9p_uart_receive(char* return_char);

/* STRUCT USED TO SEND INFORMATION OVER QUEUES */
struct rtcm_packet {
    void* fifo_reserved;
    char* message;   
    uint8_t length;
} typedef RTCM;




#endif