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

#include "hal_zedf9p.h"

static struct device * uart;

/* 0 for success
 * -1 for failure
 */
static int hal_init_zedf9p_uart() {
    
	uart = device_get_binding("UART_1");

    if (uart == NULL) {
        return -1;
    }
    
    return 0;
}

int hal_init_zedf9p() {

	if (hal_init_zedf9p_uart()) {
        return -1;
    }

	// uint8_t initialise[6] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00};

    // for (int i = 0; i < 6; i++) {
    //     uart_poll_out(uart, initialise[i]);
    // }
	
	
	// uint8_t checksumA = 0;
	// uint8_t checksumB = 0;

	// checksumA += 0x06;
	// checksumB += checksumA;

	// checksumA += 0x08;
	// checksumB += checksumA;

	// checksumA += (0 & 0xFF);
	// checksumB += checksumA;

	// checksumA += (0 >> 8);
	// checksumB += checksumA;
	// uint8_t payload[32];
	// for (uint16_t i = 0; i < 0; i++) {
	// 	checksumA += payload[i];
	// 	checksumB += checksumA;
	// }
	
	// uint8_t checksums[2] = {checksumA, checksumB};

    // uart_poll_out(uart, checksums[0]);
    // uart_poll_out(uart, checksums[1]);

    return 0;
}

int hal_zedf9p_uart_receive(char* return_char) {

    if (uart_poll_in(uart, return_char) != 0) {
        return -1;
    }

    return 0;
}





