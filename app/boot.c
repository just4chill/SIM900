#include "config.h"
#include "LPC17xx.h"
#include "system_LPC17xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stdio.h"
#include <stdint.h>
#include "string.h"
#include "stdlib.h"

#include "uart.h"
#include "gsm.h"
#include "boot.h"
#include "app.h"

void sysboot(void * pvParameters)
{
	debug_out("booting..\r\n");
	debug_out("ping modem\r\n");

	if(gsm_ping_modem())
	{
		debug_out("ping modem success\r\n");
		debug_out("checking network registration status\r\n");
		if(gsm_query_netwkregister())
		{
			debug_out("network registered\r\n");
			debug_out("query rssi\r\n");
			if(gsm_query_rssi() > _MIN_RSSI)
			{
				debug_out("rssi above threshold\r\n");
				debug_out("query gprs status\r\n");
				if(gsm_query_gprs())
				{
					debug_out("gprs enabled\r\n");
					debug_out("gprs ip status\r\n");
					gsm_query_ipstatus();
					if(strstr(gprs.ip_status, "IPINITIAL"))
					{
						debug_out("ip initial state\r\n");
						debug_out("setting apn\r\n");

						if(gsm_set_apn())
						{
							debug_out("apn set success\r\n");
							debug_out("bring wireless up\r\n");
							if(gsm_bring_wireless_up())
							{
								debug_out("bring wireless up success\r\n");
								debug_out("getting ip address\r\n");
								gsm_query_ipaddr();
								debug_out("ip: ");
								debug_out(gprs.ip_addr);
								debug_out("\r\n");
							}
							else
							{
								// TODO: Retry
								debug_out("bring wireless up failure\r\n");
							}
						}
						else
						{
							// TODO: Retry
							debug_out("apn set failure\r\n");
						}
					}
					else
					{
						debug_out("ip not initial\r\n");
						debug_out("ip shut\r\n");
						if(gsm_tcp_shut())
						{
							debug_out("ip shut success\r\n");
							debug_out("setting apn\r\n");
							if(gsm_set_apn())
							{
							debug_out("apn set success\r\n");
							debug_out("bring wireless up\r\n");
							if(gsm_bring_wireless_up())
							{
								debug_out("bring wireless up success\r\n");
								debug_out("getting ip address\r\n");
								gsm_query_ipaddr();
								debug_out("ip: ");
								debug_out(gprs.ip_addr);
								debug_out("\r\n");
							}
							else
							{
								// TODO: Retry
								debug_out("bring wireless up failure\r\n");
							}
							}
							else
							{
								// TODO: Retry
								debug_out("apn set failure\r\n");
							}						
						}
						else
						{
							// TODO: Retry
							debug_out("ip shut failure\r\n");
						}
					}
				}
				else
				{
					// TODO: Try to activate
					debug_out("gprs not enabled\r\n");
				}
			}
			else
			{
				// TODO: Try again 
				debug_out("rssi above threshold\r\n");
			}
		}
		else
		{
			// TODO: Try again 
			debug_out("network not registered\r\n");
		}
	}
	else
	{
		// TODO: Try again 
		debug_out("ping modem failure\r\n");
	}

	debug_out("booting finished\r\n");
	debug_out("creating tasks\r\n");
	xTaskCreate(display_process,
			(signed portCHAR *)"display",
			128,
			NULL,
			tskIDLE_PRIORITY,
			NULL);

	xTaskCreate(display_process,
			(signed portCHAR *)"display",
			128,
			NULL,
			tskIDLE_PRIORITY,
			NULL);

	xTaskCreate(display_process,
			(signed portCHAR *)"display",
			128,
			NULL,
			tskIDLE_PRIORITY,
			NULL);

}