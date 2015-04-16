
#ifndef __gsm_h__
#define __gsm_h__

#include "config.h"

#define HEAD 	0
#define GET  	1
#define POST 	2
#define PUT  	3
#define DELETE  4

#define modem_out(ptr) uart_print(_MODEM_PORT, ptr)
#define modem_readline(ptr) uart_readline(_MODEM_PORT, ptr)

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
	char * token[];
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

extern uint8_t gsm_tokenize(	char * ,
								char ** ,
								char ,
								char ,
								char ,
								char
				);

extern void gsm_init_tokens(uint8_t,uint8_t);
extern void gsm_free_tokens(uint8_t);

extern void gsmMalloc(uint32_t, uint32_t);

#endif