
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

void 		prvSetupHardware( void );
static void taskA(void * pvParameters);
static void taskB(void * pvParameters);
static void taskC(void * pvParameters);
void setupModem(void);

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
	uint8_t flag = 0;
	for(;;)
	{
		// gsm_read(	2,
		// 			gsm.line,
		// 			gsm.buff);
		//debug_out("Received\r\n");
		//debug_out(gsm.buff);
		//gsm_init_tokens(100,32);
		// uint8_t toks =gsm_tokenize(	gsm.buff,
		// 					gsm.token,
		// 					'\r',
		// 					':',
		// 					'\r',
		// 					','
		// 					);

		// for(uint8_t i = 0;i < toks;i++)
		// {
		// 	debug_out(gsm.token[i]);
		// 	debug_out("\r\n");
		// }
		//gsm_free_tokens(100);
		if(flag == 0)
		{
			flag = 1;
			setupModem();
		}
		vTaskDelay(1);
	}
}

const char test_buff1[] = "\r+HTTPACTION: 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100\r";

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

	// gsm_init_tokens(110,32);
	// uint8_t toks =gsm_tokenize(	test_buff1,
	// 							gsm.token,
	// 							'\r',
	// 							':',
	// 							'\r',
	// 							','
	// 							);

	// for(uint8_t i = 0;i < toks;i++)
	// {
	// 	debug_out(gsm.token[i]);
	// 	debug_out("\r\n");
	// }
	// gsm_free_tokens(110);
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

void setupModem(void)
{
	// Query modem
	modem_out("AT\r");
	gsm_read(	2,
				gsm.line,
			gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Cehck netwk registration
	modem_out("AT+CREG?\r");
	gsm_read(	2,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Query RSSI
	modem_out("AT+CSQ\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check GPRS Status
	modem_out("AT+CGATT?\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Get provider name from SIM
	modem_out("AT+CSPN?\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check network name
	modem_out("AT+COPS?\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check version
	modem_out("ATI\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check IP Status
	modem_out("AT+CIPSTATUS\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check manufacturer
	modem_out("AT+CGMM\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Check manufacturer ID
	modem_out("AT+CGMI\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Revision
	modem_out("AT+CGMR\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Serial number (IMEI)
	modem_out("AT+CGSN\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// IMSI
	modem_out("AT+CIMI\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");

	// Char set
	modem_out("AT+CSCS?\r");
	gsm_read(	4,
				gsm.line,
				gsm.buff);
	debug_out(gsm.buff);
	debug_out("\r\n");



}

