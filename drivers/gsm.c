
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "gsm.h"
#include "FreeRTOS.h"
#include "task.h"

gsmObj gsm;

void build_header(	uint8_t method,
					char * 	host,
					char * 	path,
					char * 	dat,
					char * 	buff
					){

	char len_buff[20];
	// Add first header
	switch(method)
	{
		case HEAD: 		memcpy(buff, "HEAD ",sizeof("HEAD "));
						break;
		case POST: 		memcpy(buff, "POST ",sizeof("POST "));
						break;
		case GET: 		memcpy(buff, "GET ",sizeof("GET "));
						break;
		case PUT: 		memcpy(buff, "PUT ",sizeof("PUT "));
						break;
		case DELETE: 	memcpy(buff, "DELETE ",sizeof("DELETE "));
						break;
		default:		memcpy(buff, "HEAD ",sizeof("HEAD "));
						break;
	}

	// Add path
	strcat(buff, "/");
	strcat(buff, path);	

	// Add HTTP verion info
	strcat(buff, " HTTP/1.1\r\n");

	// Add Host info
	strcat(buff, "Host: ");
	strcat(buff, host);
	strcat(buff, "\r\n");

	// Add Content-Type and Accept
	strcat(buff, "Content-Type: application/json\r\n");
	strcat(buff, "Accept: application/json\r\n");

	if(method == HEAD || method == DELETE || method == GET)
	{
		strcat(buff, "\r\n");
		return;
	}
	else if(method == PUT || method == POST)
	{
		sprintf(len_buff, "%d", strlen(dat));
		strcat(buff, "Content-Length: ");
		strcat(buff, len_buff);
		strcat(buff, "\r\n\r\n");
		strcat(buff, dat);
		strcat(buff, "\r\n\r\n");		
	}
}


void gsmMalloc(uint32_t _size, uint32_t __size)
{
	gsm.buff_size = _size;
	gsm.line_buff_size = __size;
	gsm.netwk_name = (char *) pvPortMalloc(32);
	gsm.ip_status = (char *) pvPortMalloc(64);
	gsm.ip_addr = (char *) pvPortMalloc(32);
	gsm.http_status = (char *) pvPortMalloc(4);
	gsm.buff = (char *) pvPortMalloc(gsm.buff_size);
	gsm.line = (char *) pvPortMalloc(gsm.line_buff_size);
}

uint32_t gsm_read(	uint8_t lines, 
					char * line, 
					char * buff
				)
{
	uint32_t _size = 0;
	for(uint8_t i = 0;i < lines;i++)
	{
		uint32_t _len = modem_readline(line);
		memcpy(&buff[_size], line, _len - 1);
		_size += _len - 1;
	}
	buff[_size] = '\0';
	return _size;
}


