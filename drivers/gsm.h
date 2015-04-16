
#ifndef __gsm_h__
#define __gsm_h__

#include "config.h"

#define HEAD 	0
#define GET  	1
#define POST 	2
#define PUT  	3
#define DELETE  4

#define modem_out(ptr) uart_print(_MODEM_PORT, ptr)
#define modem_readline(ptr) uart_readline(0, ptr)

typedef struct 
{
	uint8_t rssi;
	uint32_t buff_size;
	uint32_t line_buff_size;
	char * netwk_name;
	char * ip_status;
	char * ip_addr;
	char * http_status;
	char * buff;
	char * line;
}gsmObj;

extern gsmObj gsm;

extern void build_header(	uint8_t,
							char *,
							char *,
							char *,
							char * 
					);

extern uint32_t gsm_read(	uint8_t, 
							char *, 
							char *
					);

extern void gsmMalloc(uint32_t, uint32_t);

#endif