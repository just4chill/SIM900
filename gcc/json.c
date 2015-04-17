
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>



uint32_t json_build(char * buff, char ** name, uint32_t * val,uint8_t _size)
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

		if(_size == i + 1)
			strcat(buff, "\"");
		else
			strcat(buff, "\",");
	}
	strcat(buff, "}");
	return strlen(buff);
}

// uint8_t json_parse(	char *  buff,
// 					char ** token
// 				)
// {

// 	uint8_t end_flag = 0;
// 	uint32_t __size;

// 	char * _index, * _next;


// 	_next 	= index(buff, '}');


// 	uint8_t i = 1;
// 	while(!end_flag)
// 	{
// 		_index = _next + 1;
// 		_next = index(_index, ':');
// 		if(_next != NULL)
// 		{
// 			// Calculate size
// 			__size = _next - _index;

// 			// Allocate memory for first token
// 			token[i] = (char * ) malloc(__size + 2);

// 			// Copy token
// 			memcpy(token[i],_index,__size);	
// 			token[i][__size] = '\0';
// 		}
// 		else
// 		{
// 			_next = index(_index, '}');

// 			// Calculate size
// 			__size = _next - _index;

// 			// Allocate memory for first token
// 			token[i] = (char * ) malloc(__size + 2);

// 			// Copy token			
// 			memcpy(token[i],_index,__size);
// 			token[i][__size] = '\0';
// 			// Raise end flag
// 			end_flag = 1;	
// 		}
// 		// Increment
// 		i++;
// 	}
// 	// Return number of tokens
// 	return i;
// }

int main(void)
{
	char * buff;
	char * toks[12];
	char * name[10] = {"v1", "v2", "v3"};
	uint32_t val[10] 	= {1,2,3};

	buff = (char *) malloc(512);
	json_build(	buff,
				name,
				val,
				3);
	printf("%s\n", buff);

	// json_parse(buff,
	// 			toks
	// 	);

	// printf("%s\n", toks[0]);
	

	return 0;
}