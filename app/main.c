
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
#include "test.h"
#include "boot.h"
#include "app.h"

// Functions
void 		prvSetupHardware( void );

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
	debug_out("System started\r\n");

	// Init GSM Buffers
	gsm_init();
}

int main(void)
{
	/* Setup the Hardware */
	prvSetupHardware();

	xTaskCreate(sysboot,
			(signed portCHAR *)"sysboot",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY,
			NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Never reach here */
	return 0;
}

