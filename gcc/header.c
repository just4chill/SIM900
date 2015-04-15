
/* HTTP Header builder */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HEAD 	0
#define GET  	1
#define POST 	2
#define PUT  	3
#define DELETE  4

char * header;

void build_get(char * host, char * path, char * str)
{
	memcpy(str, "GET ",sizeof("GET "));
	strcat(str, "/");
	strcat(str, path);
	strcat(str, " HTTP/1.1\r\n");
	strcat(str, "Host: ");
	strcat(str, host);
	strcat(str, "\r\n");
	strcat(str, "Content-Type: application/json\r\n");
	strcat(str, "Accept: application/json\r\n\r\n");
}

void build_post(char * host, char * path, char * dat, char * str)
{
	char buf[20];
	sprintf(buf, "%d", strlen(dat));
	memcpy(str, "POST ",sizeof("POST "));
	strcat(str, "/");
	strcat(str, path);
	strcat(str, " HTTP/1.1\r\n");
	strcat(str, "Host: ");
	strcat(str, host);
	strcat(str, "\r\n");
	strcat(str, "Content-Type: application/json\r\n");
	strcat(str, "Accept: application/json\r\n");
	strcat(str, "Content-Length: ");
	strcat(str, buf);
	strcat(str, "\r\n\r\n");
	strcat(str, dat);
	strcat(str, "\r\n\r\n");
}

void build_put(char * host, char * path, char * dat, char * str)
{
	char buf[20];
	sprintf(buf, "%d", strlen(dat));
	memcpy(str, "PUT ",sizeof("POST "));
	strcat(str, "/");
	strcat(str, path);
	strcat(str, " HTTP/1.1\r\n");
	strcat(str, "Host: ");
	strcat(str, host);
	strcat(str, "\r\n");
	strcat(str, "Content-Type: application/json\r\n");
	strcat(str, "Accept: application/json\r\n");
	strcat(str, "Content-Length: ");
	strcat(str, buf);
	strcat(str, "\r\n\r\n");
	strcat(str, dat);
	strcat(str, "\r\n\r\n");
}

void build_delete(char * host, char * path, char * str)
{
	memcpy(str, "DELETE ",sizeof("DELETE "));
	strcat(str, "/");
	strcat(str, path);
	strcat(str, " HTTP/1.1\r\n");
	strcat(str, "Host: ");
	strcat(str, host);
	strcat(str, "\r\n");
	strcat(str, "Content-Type: application/json\r\n");
	strcat(str, "Accept: application/json\r\n\r\n");
}

void build_header(	char * host,
					char * path,
					char * dat,
					)


int main(void)
{
	header = (char *) malloc(512);
	build_get("google.com","weight",header);
	printf("GET Header\n%s\n", header);
	build_post("google.com","weight","{\"info\":\"1,100\"}",header);
	printf("POST Header\n%s\n", header);
	build_put("google.com","weight/1","{\"info\":\"1,100\"}",header);
	printf("PUT Header\n%s\n", header);
	build_delete("google.com","weight/1",header);
	printf("DELETE Header\n%s\n", header);
}