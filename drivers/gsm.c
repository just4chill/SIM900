
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

const char * apn_list[] = {
	"aircelgprs.pr",			// AIRCEL
	"airtelgprs.com",			// AIRTEL
	"bsnlnet",					// BSNL GPRS
	"bsnlwap",					// BSNL WAP
	"imis/internet",			// IDEA
	"rcomnet",					// RELIANCE
	"TATA.DOCOMO.INTERNET",		// TATA DOCOMO
	"www"						// VODAFONE
};


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
		if(ptr[i] != '\n' && ptr[i] != ' ' && ptr[i] != '\r' && ptr[i] != c)
		{
			gsm.buff[j++] = ptr[i];	
		}	
	}
	gsm.buff[j] = '\0';
	memcpy(ptr, gsm.buff, j + 1);
	return j;
}

uint8_t gsm_ping_modem(void)
{
	modem_out("AT\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);
	if(strstr(gsm.line[1],"OK"))
		return 1;
	else if(strstr(gsm.line[1],"ERROR"))
		return 0;
	return 0;
}

uint8_t gsm_query_operator(void)
{
	// Query Operater name
	modem_out("AT+COPS?\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize the data
	gsm_tokenize(	gsm.line[1],
				gsm.token,
				'\n',
				':',
				'\r',
				',');

	// trim and store operator name
	line_trim(gsm.token[3],'"');
	network.opr_name = (char *)pvPortMalloc(16);
	strcpy(network.opr_name,gsm.token[3]);

	// TODO: Check for errors
	return 0;
}

uint8_t gsm_query_imei(void)
{
	// Query IMEI
	modem_out("AT+CGSN\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// trim and store IMEI
	line_trim(gsm.line[1],' ');
	network.imei = (char *)pvPortMalloc(40);
	strcpy(network.imei, gsm.line[1]);

	// TODO: Check for errors
	return 0;
}

uint8_t gsm_query_imsi(void)
{
	// Query IMSI
	modem_out("AT+CIMI\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// trim and store IMSI
	line_trim(gsm.line[1], ' ');
	network.imsi = (char *)pvPortMalloc(40);
	strcpy(network.imsi, gsm.line[1]);

	// TODO: Check for errors
	return 0;	
}

uint8_t gsm_query_netwkregister(void)
{
	char * next;
	// Query Registration status
	modem_out("AT+CREG?\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// Trim and store info
	line_trim(gsm.token[2],' ');
	network.registered = (uint8_t)strtol(gsm.token[2],&next,10);
	return network.registered;
}

uint8_t gsm_query_rssi(void)
{
	char * next;
	// Query RSSI
	modem_out("AT+CSQ\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// Trim and store info
	line_trim(gsm.token[1],' ');
	network.rssi = (uint32_t)strtol(gsm.token[1],&next,10);
	return network.rssi;
}

uint8_t gsm_query_gprs(void)
{
	char * next;
	// Query GPRS Enabled/Disabled
	modem_out("AT+CGATT?\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// Trim and store info
	line_trim(gsm.token[1],' ');
	gprs.enabled = (uint8_t)strtol(gsm.token[1],&next,10);
	return gprs.enabled;
}

uint8_t gsm_set_apn(void)
{
	modem_out("AT+CSTT=\"");

	enum APN apn;

	// Check Operator name
	apn = TATADOCOMO;

	// Send APN Name
	modem_out((char *)apn_list[apn]);

	modem_out("\",\"\",\"\"\r");

	// Read 4 lines
	gsm_read(	2,
				gsm.line);

	if(strstr(gsm.line[1],"OK"))
		return 1;
	else if(strstr(gsm.line[1],"ERROR"))
		return 0;
	return 0;
}

uint8_t gsm_get_apn(void)
{
	// Query IMSI
	modem_out("AT+CSTT?\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// trim and store apn
	line_trim(gsm.token[1], ' ');
	gprs.apn = (char *)pvPortMalloc(40);
	strcpy(gprs.apn, gsm.line[1]);

	// TODO: Check for errors
	return 0;	
}

uint8_t gsm_bring_wireless_up(void)
{
	// Query IMSI
	modem_out("AT+CIICR\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);

	if(strstr(gsm.line[1],"OK"))
		return 1;
	else if(strstr(gsm.line[1],"ERROR"))
		return 0;
	return 0;
}

uint8_t gsm_query_ipstatus(void)
{
	// Query IP Status
	modem_out("AT+CIPSTATUS\r");
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[3],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// trim and store apn
	line_trim(gsm.token[1],' ');
	gprs.ip_status = (char *)pvPortMalloc(32);
	strcpy(gprs.ip_status, gsm.token[1]);

	// Process response
	return 0;
}

uint8_t gsm_activate_gprs(void)
{
	modem_out("AT+CIICR\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);
	if(strstr(gsm.line[1],"OK"))
		return 1;
	else if(strstr(gsm.line[1],"ERROR"))
		return 0;
	return 0;
}

uint8_t gsm_query_ipaddr(void)
{
	// Query IP Status
	modem_out("AT+CIFSR\r");

	// Read 4 lines
	gsm_read(	4,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[3],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// trim and store ip addr
	line_trim(gsm.token[1],' ');
	gprs.ip_addr = (char *)pvPortMalloc(32);
	strcpy(gprs.ip_addr, gsm.token[1]);

	return 0;
}

uint8_t gsm_tcp_start(char * host, char * port)
{
	// Query IP Status
	modem_out("AT+CIPSTART=\"TCP\",\"");
	modem_out(host);
	modem_out("\",\"");
	modem_out(port);
	modem_out("\"\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);


	// Trim line
	line_trim(gsm.token[1],' ');

	if(strstr(gsm.token[1],"CONNECTOK"))
		return 1;
	else if(strstr(gsm.token[1],"ALREADYCONNECT"))
		return 2;
	return 0;
}

uint8_t gsm_tcp_close(void)
{
	// Query IP Status
	modem_out("AT+CIPCLOSE\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);

	// Trim line
	line_trim(gsm.token[1],' ');

	if(strstr(gsm.token[1],"CLOSEOK"))
		return 1;
	else if(strstr(gsm.token[1],"ERROR"))
		return 0;
	return 0;
}

uint8_t gsm_tcp_shut(void)
{
	// Query IP Status
	modem_out("AT+CIPSHUT\r");

	// Read 2 lines
	gsm_read(	2,
				gsm.line);

	// Trim line
	line_trim(gsm.token[1],' ');

	if(strstr(gsm.token[1],"SHUTOK"))
		return 1;
	else if(strstr(gsm.token[1],"ERROR"))
		return 0;
	return 0;
}


uint8_t gsm_query_dns(void)
{
	modem_out("AT+CDNSCFG\r");

	// Read 6 lines
	gsm_read(	6,
				gsm.line);

	// Tokenize data
	gsm_tokenize(	gsm.line[1],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// Trim line
	line_trim(gsm.token[1],' ');
	gprs.pri_dns = (char *)pvPortMalloc(32);
	strcpy(gprs.pri_dns,gsm.token[1]);

	// Tokenize data
	gsm_tokenize(	gsm.line[2],
					gsm.token,
					'\n',
					':',
					'\r',
					',');

	// Trim line
	line_trim(gsm.token[1],' ');
	gprs.sec_dns = (char *)pvPortMalloc(32);
	strcpy(gprs.sec_dns,gsm.token[1]);

	return 0;
}

uint8_t gsm_tcp_send(char * dat)
{
	// Query IP Status
	modem_out("AT+CIPSEND\r");

	// Wait till '>' receive
	while(!uart3.rx_not_empty);
	char c = uart_getc(_MODEM_PORT);
	if(c == '>')
	{
		modem_out(dat);
		modem_putc(0x1A); // ASCII SUB
		// Wait for Response

	}
	return 0;
} 

uint32_t json_build(	char * buff, 
						char ** name, 
						uint32_t * val,
						uint8_t _size
					)
{
	char temp[20];
	uint32_t i = 0;
	memcpy(&buff[0],"{\0",2);


	for(i = 0;i < _size;i++)
	{
		strcat(buff, "\"");
		strcat(buff, name[i]);
		strcat(buff, "\":\"");
		sprintf(temp, "%d", val[i]);
		strcat(buff, temp);

		// At end
		if(_size == i + 1)
			strcat(buff, "\"");
		// Other
		else
			strcat(buff, "\",");
	}
	// End of data
	strcat(buff, "}");

	// Return size
	return strlen(buff);
}

uint8_t json_parse(	char *  buff,
					char ** token
				)
{

	uint8_t end_flag = 0;
	uint32_t __size;

	char * _index, * _next;


	_next 	= index(buff, '}');


	uint8_t i = 1;
	while(!end_flag)
	{
		_index = _next + 1;
		_next = index(_index, ':');
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
			_next = index(_index, '}');

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
