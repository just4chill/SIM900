
#ifndef __test_h__
#define __test_h__

extern void uart0_print_line(void);
extern void uart3_echo(void);
extern void uart0_echo(void);
extern void http_header_test(void);
extern void delimit_test(char*);
extern void token_test(char *,
				char,
				char,
				char,
				char
				);
extern void token_test_2(void);
extern void test_delimit(char * buff, char first, char middle, char delim);

#endif