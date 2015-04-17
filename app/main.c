
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
	vTaskDelay(10);
	debug_out("TaskA\r\n");
	for(;;)
	{
		vTaskDelay(1);
	}
}

static void taskB(void * pvParameters)
{
	vTaskDelay(20);
	debug_out("TaskB\r\n");
	for(;;)
	{
		vTaskDelay(1);
	}
}

static void taskC(void * pvParameters)
{
	vTaskDelay(30);
	debug_out("TaskC\r\n");
	uint8_t flag = 0;
	for(;;)
	{
		if(flag == 0)
		{
			flag = 1;
			setupModem();
		}
		vTaskDelay(1000);
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

	// Init GSM Buffers
	gsm_init();
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
	uint8_t token_count = 0;
	char test_buff[20];
	char * next;

	// Query modem and check response
	modem_out("AT\r");
	token_count = gsm_read(	2,
							gsm.line);


	// Query Operater name
	modem_out("AT+COPS?\r");
	gsm_read(	4,
				gsm.line);

	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	line_trim(gsm.token[3],'"');
	network.opr_name = (char *)pvPortMalloc(16);
	strcpy(network.opr_name,gsm.token[3]);
	debug_out("NETWORK: ");
	debug_out(network.opr_name);
	debug_out("\r\n");

	// Query IMEI
	modem_out("AT+CGSN\r");
	gsm_read(	4,
				gsm.line);
	line_trim(gsm.line[1],' ');
	network.imei = (char *)pvPortMalloc(40);
	strcpy(network.imei, gsm.line[1]);
	debug_out("IMEI: ");
	debug_out(network.imei);
	debug_out("\r\n");

	// Query IMSI
	modem_out("AT+CIMI\r");
	gsm_read(	4,
				gsm.line);
	line_trim(gsm.line[1], ' ');
	network.imsi = (char *)pvPortMalloc(40);
	strcpy(network.imsi, gsm.line[1]);
	debug_out("IMSI: ");
	debug_out(network.imsi);
	debug_out("\r\n");	

	// Query Registration status
	modem_out("AT+CREG?\r");
	gsm_read(	4,
				gsm.line);
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	line_trim(gsm.token[2],' ');
	network.registered = (uint8_t)strtol(gsm.token[2],&next,10);
	sprintf(test_buff,"REGI: %d\r\n",network.registered);
	debug_out(test_buff);

	// Query RSSI
	modem_out("AT+CSQ\r");
	gsm_read(	4,
				gsm.line);
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	line_trim(gsm.token[1],' ');
	network.rssi = (uint32_t)strtol(gsm.token[1],&next,10);
	sprintf(test_buff,"RSSI: %d\r\n",network.rssi);
	debug_out(test_buff);

	// Query GPRS Enabled/Disabled
	modem_out("AT+CGATT?\r");
	gsm_read(	4,
				gsm.line);

	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	line_trim(gsm.token[1],' ');
	gprs.enabled = (uint8_t)strtol(gsm.token[1],&next,10);
	sprintf(test_buff,"GPRS: %d\r\n",gprs.enabled);
	debug_out(test_buff);

	// Query IP Status
	modem_out("AT+CIPSTATUS\r");
	gsm_read(	4,
				gsm.line);

	gsm_tokenize(	gsm.line[3],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	line_trim(gsm.token[1],' ');
	gprs.ip_status = (char *)pvPortMalloc(32);
	strcpy(gprs.ip_status, gsm.token[1]);
	debug_out("IP STATUS: ");
	debug_out(gprs.ip_status);
	debug_out("\r\n");
}

