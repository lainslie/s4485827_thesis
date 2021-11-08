/**   
 ***************************************************************
 * @file    mylib/gps/src/os_zedf9p.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    12/04/2021   
 * @brief   HCI hal source file    
 ***************************************************************
 * EXTERNAL FUNCTIONS
***************************************************************
* os_init_zedf9p () - initialise gps receive therad
***************************************************************
 */

#include "os_zedf9p.h"


#define STACKSIZE 2048
#define PRIORITY 7

static void TaskZedF9PReceive(void* p1, void* p2, void* p3);

K_THREAD_STACK_DEFINE(zedf9p_receive_stack_area, STACKSIZE);

static k_tid_t zedf9p_receive_tid;

static struct k_thread zedf9p_receive_thread;

/* 
 * Function used to start gps receiver thread. 
 * Returns 0 for success
 * 
 */
int os_init_zedf9p() {

    zedf9p_receive_tid = k_thread_create(&zedf9p_receive_thread, zedf9p_receive_stack_area,\
                        K_THREAD_STACK_SIZEOF(zedf9p_receive_stack_area),\
                        TaskZedF9PReceive,\
                        NULL, NULL, NULL,\
                        PRIORITY, 0, K_NO_WAIT);
    return 0;
}


/* 
 * GPS receiver task. Processes information from GPS receiver HAL driver
 * and if data is valid, places information on queue. 
 */
static void TaskZedF9PReceive(void* p1, void* p2, void* p3) {

    char payload[MAX_PAYLOAD_SIZE];
    uint16_t length = 0;
   
    for (int i = 0;;) {
        char* receive_char = k_malloc(sizeof(char));

		int ret = hal_zedf9p_uart_receive(receive_char);
        
        
		if (ret) {
            k_free(receive_char);
            continue;
        }
        char recv = *receive_char;
        k_free(receive_char);
        

        payload[i] = recv;
        
        switch (i) {
        case 0: /* valid message is of RTCM type */
            if (recv == 0xD3) {
                i++;
            } else {
                i = 0;
            }
            break;

        case 1: /* MESSAGE LENGTH */
            length = (recv & 0x03) << 8;
            i++;
            break;
        
        case 2: /* MESSAGE LENGTH */
            length |= recv; 
            length += 6; 

            if (length > MAX_PAYLOAD_SIZE) {
                i = 0;
                break;
            }

            i++;
            break;
            
        default: /* CORRECTIONAL INFORMATION */
            if (i < (length - 1)) {
                i++;
                break;
            }

            payload[i] = recv;
            payload[i + 1] = 0;

            RTCM packet = {.length=(length), .message=(char*)payload};

            queue_mdot_transmit_message(&packet);
            k_usleep(1);
            i = 0;
            break;
        }	
    }
}


