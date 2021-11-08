/**   
 ***************************************************************
 * @file    mylib/gps/src/hal_zedf9p.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    8/11/2021   
 * @brief   UBLOX ZED-F9P hal source file    
 ***************************************************************
 * EXTERNAL FUNCTIONS
***************************************************************
* hal_init_zedf9p () - initialise all states and hardware for zedf9p
* hal_zedf9p_uart_receive()  - receive data from zedf9p
***************************************************************
 */

#include "hal_zedf9p.h"

static struct device * uart;

/* 
 * Static helper function used to initialise the respective uart
 * for the UBLOX ZED-F9P. Function returns -1 for failure
 * and 0 for success.
 */
static int hal_init_zedf9p_uart() {

    /* macro defined in hal_zedf9.h to allow for adaptability */
	uart = device_get_binding(ZED_UART_DRIVER);

    if (uart == NULL) {
        return -1;
    }
    
    return 0;
}


/*
 * Function used to intialise zedf9p. Uart firstly enabled
 * then device is polled for information to ensure 
 * validity. Return -1 for failure, 0 for success.
 */
int hal_init_zedf9p() {

	uint8_t initialise[6] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00};
	uint8_t checksumA = 0;
	uint8_t checksumB = 0;
	uint8_t payload[32];

	if (hal_init_zedf9p_uart()) {
        return -1;
    }

    for (int i = 0; i < 6; i++) {
        uart_poll_out(uart, initialise[i]);
    }
	
	checksumA += 0x06;
	checksumB += checksumA;

	checksumA += 0x08;
	checksumB += checksumA;

	checksumA += (0 & 0xFF);
	checksumB += checksumA;

	checksumA += (0 >> 8);
	checksumB += checksumA;
	
	for (int i = 0; i < 0; i++) {
		checksumA += payload[i];
		checksumB += checksumA;
	}
	
	uint8_t checksums[2] = {checksumA, checksumB};

    uart_poll_out(uart, checksums[0]);
    uart_poll_out(uart, checksums[1]);

    return 0;
}


/*
 * Function used to receive data from ZEDF9P device.
 * Returns -1 for failure, 0 for success.
 */
int hal_zedf9p_uart_receive(char* return_char) {

    if (uart_poll_in(uart, return_char) != 0) {
        return -1;
    }

    return 0;
}





