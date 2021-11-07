/**   
 ***************************************************************
 * @file    apps/p3/src/main.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/03/2021   
 * @brief   Main file enabling all functionality for different devices
 ***************************************************************
 */

#include "main.h"
#include "os_zedf9p.h"
#include "os_mdot.h"







void main(void) {

	int ret = -1;

	// struct device* adev = device_get_binding("UART_1");
	// char test = 'a';

	// char buffer[20] = "stressed";
	// int i = 0;
	// while (1) {
	// for (int i = 0; i < 10; i++) {
	// 	uart_poll_out(adev, buffer[i]);
	// 	k_msleep(100);


	// }
	// }
	

	do {
		ret = 0;
		ret += hal_init_mdot();
		ret += hal_init_zedf9p();
		ret = 0;
		k_msleep(1000);
	} while (ret);

	os_init_mdot();
	os_init_zedf9p();
}


