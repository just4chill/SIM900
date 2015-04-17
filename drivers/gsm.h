
#ifndef __gsm_h__
#define __gsm_h__

#include "config.h"

#define HEAD 	0
#define GET  	1
#define POST 	2
#define PUT  	3
#define DELETE  4

#define modem_out(ptr) uart_print(_MODEM_PORT, ptr)
#define modem_putc(ptr) uart_putc(_MODEM_PORT, ptr)
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
	char * apn;
	char * pri_dns;
	char * sec_dns;
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

enum APN
{
	AIRCEL,
	AIRTEL,
	BSNLGPRS,
	BSNLWAP,
	IDEA,
	RELIANCE,
	TATADOCOMO,
	VODAFONE
};

extern NETWORK network;
extern GPRS gprs;
extern HTTP http;
extern GSM gsm;

// HTTP Header Builder
extern void build_header(	uint8_t,
							char *,
							char *,
							char *,
							char * 
					);

// GSM Read multiline
extern uint32_t gsm_read(	uint8_t, 
							char **
					);

// Tokenize the string
extern uint8_t gsm_tokenize(	char * ,
								char ** ,
								char ,
								char ,
								char ,
								char
				);

extern uint8_t json_parse(	char * ,
							char **
				);


extern uint32_t json_build(	char *, 
							char **, 
							uint32_t *,
							uint8_t
					);

// Init Buffers
extern void gsm_init(void);

// Trim for ' ','\r','\n'
extern uint8_t line_trim(char *,char);

// Comm functions
extern uint8_t gsm_ping_modem(void);
extern uint8_t gsm_query_operator(void);
extern uint8_t gsm_query_imei(void);
extern uint8_t gsm_query_imsi(void);
extern uint8_t gsm_query_netwkregister(void);
extern uint8_t gsm_query_rssi(void);
extern uint8_t gsm_query_gprs(void);
extern uint8_t gsm_set_apn(void);
extern uint8_t gsm_get_apn(void);
extern uint8_t gsm_bring_wireless_up(void);
extern uint8_t gsm_query_ipstatus(void);
extern uint8_t gsm_activate_gprs(void);
extern uint8_t gsm_query_ipaddr(void);
extern uint8_t gsm_tcp_start(char *, char *);
extern uint8_t gsm_tcp_close(void);
extern uint8_t gsm_tcp_shut(void);
extern uint8_t gsm_query_dns(void);
extern uint8_t gsm_tcp_send(char *);


#endif