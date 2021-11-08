/**   
 ***************************************************************
 * @file    apps/thesis/src/main.c    
 * @author  Lewis Ainslie - s4485827  
 * @date    08/11/2021   
 * @brief   Main file enabling all functionality for different devices
 ***************************************************************
 */

#include "main.h"
#include "os_zedf9p.h"
#include "os_mdot.h"


/* 
 * Main function initialising all hardware and threads.
 */
void main(void) {

	int ret;

	do {
		ret = 0;
		ret += hal_init_mdot();
		ret += hal_init_zedf9p();
		k_msleep(1000);
	} while (ret);

	os_init_mdot();
	os_init_zedf9p();
}


