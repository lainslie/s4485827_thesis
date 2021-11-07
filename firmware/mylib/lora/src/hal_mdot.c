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

#include "hal_mdot.h"


static struct device * uart;

/* 0 for success
 * -1 for failure
 */
static int hal_init_mdot_uart() {
    
	uart = device_get_binding("UART_2");

    if (uart == NULL) {
        return -1;
    }
    
    return 0;
}



static void hal_uart_transmit(char* output) {

    for (int i = 0; i < strlen(output); i++) {
        uart_poll_out(uart, output[i]);
    }
}


/* 0 for success
 * -1 for failure
 */
static int hal_uart_receive_ack(int timeout) {
    char stringer[128];

    uint64_t time = k_uptime_get();

    char ack_string[2];
    int counter = 0;

    for (int i = 0; i >= 0;) {

		unsigned char recvChar;
        
		if (uart_poll_in(uart, &recvChar) == 0) {
			time = k_uptime_get();
			ack_string[i] = recvChar;
            stringer[counter] = recvChar;
            counter++;
            i = (i + 1) % 2;
		}

        if ( (ack_string[0] == 'O') && (ack_string[1] == 'K') ) {
            return 0;
        }

        if ( (ack_string[1] == 'O') && (ack_string[0] == 'K') ) {
            return 0;
        }

        if (!timeout) {
            return 0;
        }

		if (k_uptime_get() - time > MDOT_TIMEOUT) {
            return -1;
		}
	}
    return 0;
}


int hal_mdot_start() {
    char command[64];
    int ret = 0;
    int timeout = 1;

    sprintf(command, "+PN=1");
    ret += hal_mdot_send_at(command, 0);
    
    k_msleep(1000);

    sprintf(command, "+NJM=1");
    ret += hal_mdot_send_at(command, 0);
    
    k_msleep(1000);

    sprintf(command, "+JR=5");
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+AP");
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+NI=0,%s", NETWORK_ID);
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+NK=0,%s", NETWORK_KEY);
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+TXP=20");
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+TXDR=DR4");
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    sprintf(command, "+FSB=2");
    ret += hal_mdot_send_at(command, timeout);
    
    k_msleep(1000);

    
    sprintf(command, "+JOIN");
    ret += hal_mdot_send_at(command, 2 * timeout);

    k_msleep(1000);

    
    sprintf(command, "+SEND=hello");
    ret += hal_mdot_send_at(command, timeout);

    k_msleep(2000);

    
    sprintf(command, "+SEND=hello");
    ret += hal_mdot_send_at(command, timeout);


    
    return ret;
}

int hal_init_mdot() {

    if (hal_init_mdot_uart()) {
        return -1;
    }

    return hal_mdot_start();
}

/* 0 for success
 * -1 for failure
 */
int hal_mdot_send_at(char* command, int timeout) {

    int command_len = strlen(command);

    char* send_array = k_malloc(sizeof(char) * (command_len + AT_BASE_LEN));

    sprintf(send_array, "AT%s\n", command);
    hal_uart_transmit(send_array);
    
    k_free(send_array);
    
    return hal_uart_receive_ack(timeout);
}

int hal_mdot_send_payload(RTCM* payload) {

    int payload_len = payload->length;

    char command_prefix[] = "+SENDB=";
    char hex_string[2 * payload_len + 1];

    char* command = k_malloc(sizeof(char) * 2 * (payload_len + strlen(command_prefix) + 1));

    char* ptr = &hex_string[0];
    for (int i = 0; i < payload_len; i++) {
        ptr += sprintf(ptr, "%02x", (int)payload->message[i]);
    }
    *ptr = 0;

    memcpy(command, command_prefix, strlen(command_prefix));

    memcpy(&command[strlen(command_prefix)], hex_string, strlen(hex_string) + 1);
   
    hal_mdot_send_at("AT+TXDR=DR5", 0);
    int ret = hal_mdot_send_at(command, 0);
    k_free(command);
    
    
    return ret;
}

