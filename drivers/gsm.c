
#include <stdint.h>
#include "stdlib.h"
#include "string.h"
#include "uart.h"
#include "gsm.h"
#include "FreeRTOS.h"
#include "task.h"

NETWORK network;
GPRS gprs;
HTTP http;
GSM gsm;

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


uint32_t gsm_read(	uint8_t num_lines, 
					char ** line
				)
{
	uint32_t _size = 0;
	for(uint8_t i = 0;i < num_lines;i++)
	{
		vPortFree(line[i]);
		uint32_t _len = modem_readline(gsm.buff);
		line[i] = pvPortMalloc(_len);
		memcpy(line[i], gsm.buff, _len);
		_size += _len;
	}
	return _size;
}


uint8_t gsm_tokenize(	char * buff,
						char ** token,
						char __start,
						char __middle,
						char __end,
						char __delimit
				)
{
	uint8_t end_flag = 0;
	uint32_t __size;
	char * _index, * _rindex, * _next;
	_index 	= index(buff, __start);
	_next	= index(_index, __middle);

	// Calculate size
	__size = _next - _index;

	// Allocate memory for first token
	token[0] = (char * ) pvPortMalloc(__size + 2);

	// Copy token
	memcpy(token[0], _index, __size);
	token[0][__size] = '\0';

	//debug_out(token[0]);

	uint8_t i = 1;
	while(!end_flag)
	{
		_index = _next + 1;
		_next = index(_index, __delimit);
		if(_next != NULL)
		{
			// Calculate size
			__size = _next - _index;

			// Allocate memory for first token
			token[i] = (char * ) pvPortMalloc(__size + 2);

			// Copy token
			memcpy(token[i],_index,__size);	
			token[i][__size] = '\0';
		}
		else
		{
			_next = index(_index, __end);

			// Calculate size
			__size = _next - _index;

			// Allocate memory for first token
			token[i] = (char * ) pvPortMalloc(__size + 2);

			// Copy token			
			memcpy(token[i],_index,__size);
			token[i][__size] = '\0';
			// Raise end flag
			end_flag = 1;	
		}
		// Increment
		i++;
	}
	// Return number of tokens
	return i;
}

void gsm_init(void)
{
	// Initiate 512 bytes buffer for general purpose
	gsm.buff = (char *) pvPortMalloc(512);
}

uint8_t line_trim(char * ptr, char c)
{
	uint8_t i;
	uint8_t j = 0;
	for(i = 0;ptr[i] != '\0';i++)
	{
		if(ptr[i] != '\n' && ptr[i] != ' ' && ptr[i] != c)
		{
			gsm.buff[j++] = ptr[i];	
		}	
	}
	gsm.buff[j] = '\0';
	memcpy(ptr, gsm.buff, j+1);
	return j;
}


