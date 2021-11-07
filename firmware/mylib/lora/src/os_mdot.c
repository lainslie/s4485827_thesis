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


#include "os_mdot.h"


#define STACKSIZE 2048

/* scheduling priority used by each thread */
#define PRIORITY 7

void TaskMdotTransmit(void* p1, void* p2, void* p3);

K_THREAD_STACK_DEFINE(mdot_transmit_stack_area, STACKSIZE);


K_MEM_SLAB_DEFINE(mdot_transmit_queue_slab, sizeof(RTCM), 10, 0);
K_FIFO_DEFINE(mdot_transmit_queue);

static k_tid_t mdot_transmit_tid;

static struct k_thread mdot_transmit_thread;

/* 0 for success
 * -1 for failure
 */
int os_init_mdot() {

    mdot_transmit_tid = k_thread_create(&mdot_transmit_thread, mdot_transmit_stack_area,\
                        K_THREAD_STACK_SIZEOF(mdot_transmit_stack_area),\
                        TaskMdotTransmit,\
                        NULL, NULL, NULL,\
                        PRIORITY, 0, K_NO_WAIT);
    return 0;
}


/* Logging task. Task checks semaphores and determines the print type accordingly.
 * Depending on the print type, if the message received off the fifo is also
 * of the same type, that message printing will be handle in 'print_log_data'
 * */
void TaskMdotTransmit(void* p1, void* p2, void* p3) {

	while (1) {

        RTCM* sending_data;
    
        if (k_fifo_is_empty(&mdot_transmit_queue)) {
            k_msleep(100);
            continue;
        }

        sending_data = k_fifo_get(&mdot_transmit_queue, K_NO_WAIT);
        // printf("data: %s", sending_data->message);

        int ret = hal_mdot_send_payload(sending_data);


        k_free(sending_data->message);


        /* free allocated memory */
        if (sending_data != NULL) {
            k_mem_slab_free(&mdot_transmit_queue_slab, (void **)&sending_data);
        }

		k_msleep(100);
	}
}



/* AHU intialisation of tasks and all hardware requirements
 * */
void queue_mdot_transmit_message(RTCM *data) {

    RTCM *send_data;
    int err = 0;

    if (err = k_mem_slab_alloc(&mdot_transmit_queue_slab, (void **)&send_data, K_FOREVER) != 0) {
        return;
    }

    send_data->length = data->length;

    send_data->message = k_malloc(sizeof(char) * send_data->length);
    memcpy(send_data->message, data->message, send_data->length);

    k_fifo_put(&mdot_transmit_queue, send_data);
}
