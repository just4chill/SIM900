
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

void 		prvSetupHardware( void );
static void taskA(void * pvParameters);
static void taskB(void * pvParameters);
static void taskC(void * pvParameters);

void uart0_echo(void)
{
	while(uart0.num_bytes > 0)
	{
		char c = uart_getc(0);
		uart_puts(0, &c,1);
	}	
}

void uart3_echo(void)
{
	while(uart3.num_bytes > 0)
	{
		char c = uart_getc(3);
		uart_puts(3, &c,1);
	}	
}

void uart0_print_line(void)
{
	gsm_read(3, gsm.line, gsm.buff);
	debug_out("\r\n");
	debug_out(gsm.buff);
	debug_out("\r\n");
}

static void taskA(void * pvParameters)
{
	for(;;)
	{
		;
	}
}

static void taskB(void * pvParameters)
{
	debug_out("TaskB\r\n");
	for(;;)
	{
		uart3_echo();
		vTaskDelay(1);
	}
}

static void taskC(void * pvParameters)
{
	debug_out("TaskC\r\n");
	for(;;)
	{
		uart0_print_line();
		//debug_out("Line Feed\r\n");
		//debug_out(line);
		vTaskDelay(1);
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
	debug_out("System started\r\n");
	modem_out("AT\r");
	

	gsmMalloc(GSM_RX_BUFFER_SIZE, GSM_LINE_BUFFER_SIZE);

	build_header(	HEAD,
					"google.com",
					"search",
					NULL,
					gsm.buff
					);

	debug_out("HEAD HEADER\r\n");
	debug_out(gsm.buff);

	build_header(	GET,
					"google.com",
					"search",
					NULL,
					gsm.buff
					);

	debug_out("GET HEADER\r\n");
	debug_out(gsm.buff);

	build_header(	POST,
					"google.com",
					"search",
					"{\"info\":\"1,200\"}",
					gsm.buff
					);

	debug_out("POST HEADER\r\n");
	debug_out(gsm.buff);

	build_header(	PUT,
					"google.com",
					"search/1",
					"{\"info\":\"1,200\"}",
					gsm.buff
					);

	debug_out("PUT HEADER\r\n");
	debug_out(gsm.buff);

	build_header(	DELETE,
					"google.com",
					"search/1",
					NULL,
					gsm.buff
					);

	debug_out("DELETE HEADER\r\n");
	debug_out(gsm.buff);

	//line = (char *) pvPortMalloc(64);
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

