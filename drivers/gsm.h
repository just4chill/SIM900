
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

// Dynamic allocation
typedef struct 
{
	uint8_t 	rssi;
	uint8_t		registered;
	char 	* 	opr_name;
	char 	* 	imei;
	char 	* 	imsi;
}NETWORK;

// Dynamic allocation
typedef struct 
{
	uint8_t enabled;
	char * ip_status;
	char * ip_addr;	
}GPRS;

// Dynamic allocation 
typedef struct 
{
	char * status;
	char * header;	
	char * data;
}HTTP;

// Dynamic allocation
typedef struct 
{
	char * buff;
	char * line[127];
	char * token[127]	
}GSM;

extern NETWORK network;
extern GPRS gprs;
extern HTTP http;
extern GSM gsm;


extern void build_header(	uint8_t,
							char *,
							char *,
							char *,
							char * 
					);

extern uint32_t gsm_read(	uint8_t, 
							char **
					);

extern uint8_t gsm_tokenize(	char * ,
								char ** ,
								char ,
								char ,
								char ,
								char
				);

extern void gsm_init(void);
extern uint8_t line_trim(char *,char);

#endif