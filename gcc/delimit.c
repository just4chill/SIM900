
/* Delimit test using strtok */
#include <stdio.h>
#include <string.h>

char str[] = "\"obj\":\"value\"";

void tokenize(char * raw)
{
	char * tok = strtok(raw,"\"");
	while(tok != NULL)
	{
		printf("str: %s\n",tok);
		tok = strtok(NULL,"\"");
	}
}

int main(void)
{
	// printf("str: %s\n",str);
	tokenize(str);
	return 0;
}
