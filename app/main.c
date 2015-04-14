
#include "config.h"
#include "LPC17xx.h"
#include "system_LPC17xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "gsm.h"
#include "printf_stdarg.h"


void 		prvSetupHardware( void );
static void taskA(void * pvParameters);
static void taskB(void * pvParameters);
static void taskC(void * pvParameters);

static void taskA(void * pvParameters)
{
	xQueueHandle xQueue1;
	xQueue1 = xQueueCreate(10, sizeof(char));
	if(xQueue1 == 0)
		debug_out("Queue1 create failed\r\n");
	else
		debug_out("Queue1 create success\r\n");


	for(;;)
	{
		;
	}
}

static void taskB(void * pvParameters)
{
	xQueueHandle xQueue2;
	xQueue2 = xQueueCreate(10, sizeof(char));
	if(xQueue2 == 0)
		debug_out("Queue2 create failed\r\n");
	else
		debug_out("Queue2 create success\r\n");

	for(;;)
	{
		;
	}
}

static void taskC(void * pvParameters)
{
	xQueueHandle xQueue3;
	xQueue3 = xQueueCreate(10, sizeof(char));
	if(xQueue3 == 0)
		debug_out("Queue3 create failed\r\n");
	else
		debug_out("Queue3 create success\r\n");

	for(;;)
	{
		;
	}
}


void prvSetupHardware( void )
{
	/* Initialte Clock */
	SystemInit();
	SystemCoreClockUpdate();

	// Init UARTs
	uart_init(_DEBUG_PORT, _DEBUG_BAUD_RATE);				// Debug port
	uart_init(_MODEM_PORT, _MODEM_BAUD_RATE);				// Modem

	// Init FIFOs
	uart_init_fifo(_MODEM_PORT, MODEM_UART_FIFO_SIZE);	
	uart_init_fifo(_DEBUG_PORT, DEBUG_UART_FIFO_SIZE);	

	printf("System started\r\n");
}

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(taskA,
			(signed portCHAR *)"taskA",
			128,
			NULL,
			3,
			NULL);

	xTaskCreate(taskB,
			(signed portCHAR *)"taskB",
			128,
			NULL,
			4,
			NULL);

	xTaskCreate(taskC,
			(signed portCHAR *)"taskC",
			128,
			NULL,
			5,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

