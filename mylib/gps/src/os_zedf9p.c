/**   
 ***************************************************************
 * @file    mylib/hci/include/hal_hci.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    12/04/2021   
 * @brief   HCI hal source file    
 ***************************************************************
 * EXTERNAL FUNCTIONS
***************************************************************
* hal_init_scu () - initialise i2c communication for scu
* hal_handle_scu_job()  - handle i2c communcation for scu
***************************************************************
 */

#include "os_zedf9p.h"



#define STACKSIZE 2048

/* scheduling priority used by each thread */
#define PRIORITY 7

void TaskZedF9PReceive(void* p1, void* p2, void* p3);

K_THREAD_STACK_DEFINE(zedf9p_receive_stack_area, STACKSIZE);


static k_tid_t zedf9p_receive_tid;

static struct k_thread zedf9p_receive_thread;

/* 0 for success
 * -1 for failure
 */
int os_init_zedf9p() {

    zedf9p_receive_tid = k_thread_create(&zedf9p_receive_thread, zedf9p_receive_stack_area,\
                        K_THREAD_STACK_SIZEOF(zedf9p_receive_stack_area),\
                        TaskZedF9PReceive,\
                        NULL, NULL, NULL,\
                        PRIORITY, 0, K_NO_WAIT);
    return 0;
}


/* Logging task. Task checks semaphores and determines the print type accordingly.
 * Depending on the print type, if the message received off the fifo is also
 * of the same type, that message printing will be handle in 'print_log_data'
 * */
void TaskZedF9PReceive(void* p1, void* p2, void* p3) {

    char payload[MAX_PAYLOAD_SIZE];
   
    for (int i = 0; i < 50; i++) {
        payload[i] = i + 1;

    }

    uint16_t length = 0;
    char messager[53];
    for (int i = 0; i < 53; i++) {
        messager[i] = i + 1;
    }
    
    for (int i = 0;;) {
        char* receive_char = k_malloc(sizeof(char));

		int ret = hal_zedf9p_uart_receive(receive_char);
        
        
		if (ret) {
            k_free(receive_char);
            continue;
        }
        char recv = *receive_char;
        // printk("receive %c\n", recv);
        k_free(receive_char);
        

        payload[i] = recv;
        
        switch (i) {

            case 0:
                if (recv == 0xD3) {
                    i++;
                } else {
                    i = 0;
                }
                break;

            case 1:
                length = (recv & 0x03) << 8;
                
                i++;
                break;
            
            case 2:
                length |= recv; //Bits 0-7 of packet length
                length += 6;

                if (length > 120) {
                    i = 0;
                    break;
                }

                i++;
                break;
                
            default:   
                if (i < (length - 1)) {
                    i++;
                    break;
                }

                payload[i] = recv;
                payload[i + 1] = 0;
                // if (length > 100) {
                //     i = 0;
                //     break;
                // }

                RTCM packet = {.length=(length), .message=(char*)payload};
                
                // printk("sending message %d \n", length);
                queue_mdot_transmit_message(&packet);
                k_usleep(1);
                

                i = 0;
                break;
        }	
        // k_usleep(1);
        
    }

}


