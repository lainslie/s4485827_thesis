/**   
 ***************************************************************
 * @file    mylib/lora/src/os_mdot.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/11/2021   
 * @brief   OS mDot source file    
 ***************************************************************
 * EXTERNAL FUNCTIONS
***************************************************************
* os_init_mdot () - function used to initialise Lora thread
* queue_mdot_transmit_message()  - handle addition of messages to queue
***************************************************************
 */


#include "os_mdot.h"


#define STACKSIZE 2048
#define PRIORITY 7

static void TaskMdotTransmit(void* p1, void* p2, void* p3);

K_THREAD_STACK_DEFINE(mdot_transmit_stack_area, STACKSIZE);
K_MEM_SLAB_DEFINE(mdot_transmit_queue_slab, sizeof(RTCM), 10, 0);
K_FIFO_DEFINE(mdot_transmit_queue);

static k_tid_t mdot_transmit_tid;
static struct k_thread mdot_transmit_thread;

/*
 * Function used to start Lora transmission thread
 */
int os_init_mdot() {

    mdot_transmit_tid = k_thread_create(&mdot_transmit_thread, mdot_transmit_stack_area,\
                        K_THREAD_STACK_SIZEOF(mdot_transmit_stack_area),\
                        TaskMdotTransmit,\
                        NULL, NULL, NULL,\
                        PRIORITY, 0, K_NO_WAIT);
    return 0;
}


/* 
 * LoraWAN transmission task. Receives data off queue, from GPS
 * Receiver task, and uses HAL to transmit data
 */
static void TaskMdotTransmit(void* p1, void* p2, void* p3) {
    int ret;

	while (1) {
        RTCM* sending_data;
    
        if (k_fifo_is_empty(&mdot_transmit_queue)) {
            k_msleep(100);
            continue;
        }

        sending_data = k_fifo_get(&mdot_transmit_queue, K_NO_WAIT);

        ret = hal_mdot_send_payload(sending_data);

        /* free allocated memory */
        k_free(sending_data->message);
        if (sending_data != NULL) 
            k_mem_slab_free(&mdot_transmit_queue_slab, (void **)&sending_data);

		k_msleep(100);
	}
}




/*
 * Function used to add RTCM data onto the mdot
 * transmission queue. Handles memory 
 * allocation
 */
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
