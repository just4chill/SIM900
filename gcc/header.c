
/* HTTP Header builder */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HEAD 	0
#define GET  	1
#define POST 	2
#define PUT  	3
#define DELETE  4

char * header;

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

void testHeader(void)
{
	build_header(	HEAD,
					"google.com",
					"search",
					NULL,
					header
					);
	printf("HEAD\n%s\n",header);

	build_header(	GET,
					"google.com",
					"search?q=test",
					NULL,
					header
					);
	printf("GET\n%s\n",header);

	build_header(	POST,
					"google.com",
					"search",
					"{\"info\":\"1,100\"}",
					header
					);
	printf("POST\n%s\n",header);

	build_header(	PUT,
					"google.com",
					"search",
					"{\"info\":\"1,100\"}",
					header
					);
	printf("PUT\n%s\n",header);

	build_header(	DELETE,
					"google.com",
					"search",
					NULL,
					header
					);
	printf("DELETE\n%s\n",header);

}


int main(void)
{
	header = (char *) malloc(512);
	testHeader();
	return 0;
}