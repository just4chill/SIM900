
#include <stdint.h>
#include "stdlib.h"
#include "string.h"
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
		memcpy(&buff[_size], line, _len);
		_size += _len;
	}
	buff[_size] = '\0';
	return _size;
}


void gsm_init_tokens(uint8_t num, uint8_t _size)
{
	for(uint8_t i = 0;i < num;i++)
	{
		gsm.token[i] = (char *) pvPortMalloc(_size);
	}
}

void gsm_free_tokens(uint8_t num)
{
	for(uint8_t i = 0;i < num;i++)
	{
		vPortFree(gsm.token[i]);
	}	
}

char * StringTrim(char * *pointerToString)
{
    uint8_t start=0, length=0;

        // Trim.Start:
        length = strlen(*pointerToString);
        while ((*pointerToString)[start]==' ') start++;
        (*pointerToString) += start;

        if (start < length) // Required for empty (ex. "    ") input
        {
            // Trim.End:
            uint8_t end = strlen(*pointerToString)-1; // Get string length again (after Trim.Start)
            while ((*pointerToString)[end]==' ') end--;
            (*pointerToString)[end+1] = 0;
        }

    return *pointerToString;
}

uint8_t gsm_tokenize(	char * buff,
						char ** tokens,
						char __start,
						char __middle,
						char __end,
						char __delimit
				)
{
	uint8_t end_flag = 0;
	char * _index, * _rindex, * _next;
	_index 	= index(buff, __start);
	_next	= index(_index, __middle);
	//debug_out("token init\r\n");
	//debug_out(_index);
	memcpy(tokens[0], _index, _next - _index);
	memcpy(&tokens[0][_next - _index],'\0',1);

	uint8_t i = 1;
	while(!end_flag)
	{
		_index = _next + 1;
		_next = index(_index, __delimit);
		if(_next != NULL)
		{
			memcpy(tokens[i],_index,_next - _index);	
			memcpy(&tokens[i][_next - _index],'\0',1);
			//StringTrim(&tokens[i]);
			//tokens[i] = trim(tokens[i]);
		}
		else
		{
			_next = index(_index, __end);
			memcpy(tokens[i],_index,_next - _index);
			memcpy(&tokens[i][_next - _index],'\0',1);
			//tokens[i] = trim(tokens[i]);
			//StringTrim(&tokens[i]);
			end_flag = 1;	
		}
		i++;
	}
	return i;
}


