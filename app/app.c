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

void display_process(void * pvParameters)
{
	debug_out("display task started\r\n");
	for(;;)
	{
		vTaskDelay(1);
	}
}

void http_process(void * pvParameters)
{
	debug_out("http task started\r\n");
	for(;;)
	{
		vTaskDelay(1);
	}
}

void fetch_data_process(void * pvParameters)
{
	debug_out("http task started\r\n");
	for(;;)
	{
		vTaskDelay(1);
	}
}