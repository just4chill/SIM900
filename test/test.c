
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

void http_header_test(void)
{
	// build_header(	HEAD,
	// 				"google.com",
	// 				"search",
	// 				NULL,
	// 				gsm.buff
	// 				);

	// debug_out("HEAD HEADER\r\n");
	// debug_out(gsm.buff);

	// build_header(	GET,
	// 				"google.com",
	// 				"search",
	// 				NULL,
	// 				gsm.buff
	// 				);

	// debug_out("GET HEADER\r\n");
	// debug_out(gsm.buff);

	// build_header(	POST,
	// 				"google.com",
	// 				"search",
	// 				"{\"info\":\"1,200\"}",
	// 				gsm.buff
	// 				);

	// debug_out("POST HEADER\r\n");
	// debug_out(gsm.buff);

	// build_header(	PUT,
	// 				"google.com",
	// 				"search/1",
	// 				"{\"info\":\"1,200\"}",
	// 				gsm.buff
	// 				);

	// debug_out("PUT HEADER\r\n");
	// debug_out(gsm.buff);

	// build_header(	DELETE,
	// 				"google.com",
	// 				"search/1",
	// 				NULL,
	// 				gsm.buff
	// 				);

	// debug_out("DELETE HEADER\r\n");
	// debug_out(gsm.buff);
}

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

void token_test(char * buff,
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
	uart_puts(0, _index, _next - _index);
	debug_out("\r\n");

	while(!end_flag)
	{
		_index = _next + 1;
		_next = index(_index, __delimit);
		if(_next != NULL)
		{
			uart_puts(0, _index, _next - _index);
			debug_out("\r\n");				
		}
		else
		{
			_next = index(_index, __end);
			uart_puts(0, _index, _next - _index);
			debug_out("\r\n");
			debug_out("End\r\n");
			end_flag = 1;	
		}
	}

	debug_out("Finished\r\n");
	debug_out("\r\n");
}

void delimit_test(char * raw)
{
	//char raw[64] = "\rHello,World,Hai\r";
	char buff[20];
	char * first, * next;


	first = index(raw, '\r');
	next  = index(first, ',');
	uart_puts(0, first + 1, ((next - first) - 1) );
	debug_out("\r\n");

	first = next;
	next  = index(first + 1, ',');
	uart_puts(0, first + 1, ((next - first) - 1) );
	debug_out("\r\n");

	first = next;
	next  = index(first + 1, '\r');
	uart_puts(0, first + 1, ((next - first) - 1) );
	debug_out("\r\n");
}

char test_buff[] = "\rHow,are,you?\r";

void token_test_2(void)
{
	char * first, * next;
	debug_out("1st: ");
	first = memchr(test_buff, '\r', sizeof(test_buff));
	uart_puts(0, first + 1, (test_buff - first));
	debug_out("\r\n");

	next = memchr(first, ',', test_buff - first);
	next = first;
	uart_puts(0, first + 1, (test_buff - first));
	debug_out("\r\n");
}

void uart0_print_line(void)
{
	// char * ptr;
	// char buff[20];
	// char * next;
	// uint32_t len = uart_readline(0, gsm.line);
	// ptr = memchr(gsm.line, ':', len);
	// uint32_t num = strtol(ptr + 2,&next,10);
	// sprintf(buff, "num: %d",num);
	// debug_out("Raw: ");
	// debug_out(ptr);
	// debug_out("\r\n");
	// debug_out(buff);
	// debug_out("\r\n");
	// debug_out("Next: ");
	// debug_out(next);
	// debug_out("\r\n");
	//gsm_read(2);
	uint32_t _size = gsm_read(	2,
								gsm.line,
								gsm.buff);
	// debug_out(gsm.buff);
	// debug_out("\r\n");

	char * _firstcr = memchr(gsm.buff,'\r',_size);
	// if(_firstcr != NULL)
	// {
		// debug_out("\r\n");
		// debug_out(_firstcr);
		if(_firstcr == NULL)
			debug_out("Not found\r\n");	
		else
		{
			debug_out("found\r\n");
			char * c = memchr(_firstcr,'a',_size - 1);
			if(c == NULL)
				debug_out("Not found\r\n");
			else
				debug_out("found\r\n");
		}
//	}
	// char * _index 	= memchr(_firstcr + 1,',',(gsm.buff - _firstcr));
	// if(_index != NULL)
	// {
	// 	debug_out("\r\n");
	// 	debug_out(_index);		
	// }

	
	// debug_out("\r\n");
	// debug_out(_index);
	// debug_out("\r\n");
	// debug_out(_index);
	// debug_out("\r\n");
	// uart_puts(0, _firstcr, (_index - _firstcr));

	// char * __index = memchr(_index + 1,',',_size - 1);
	
	// debug_out(_index);
	// debug_out("\r\n");
	// debug_out(__index);
	// debug_out("\r\n");
	//delimit_test(gsm.buff);
}

void test_delimit(char * buff, char first, char middle, char delim)
{
	char * _first;
	char * _next;

	_first = index(buff, first) + 1;
	_next  = index(_first, middle);
	uart_puts(0, _first, (_next - _first));
	debug_out("\r\n");
	_first = _next + 1;
	_next  = index(_first, delim) + 1;
	uart_puts(0, _first, (_next - _first));
	debug_out("\r\n");
	_first = _next;
	_next  = index(_first, first) + 1;
	uart_puts(0, _first, (_next - _first));
	debug_out("\r\n");

}