
#ifndef __uart_h__
#define __uart_h__

#include <stdint.h>

#define debug_out(ptr) uart_print(0, ptr)	

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04
#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01
#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80
#define BUFSIZE		0x40
#define TEMT 		(1 << 6) 

typedef struct
{
	uint32_t 			i_first;
	uint32_t 			i_last;
	uint8_t				rx_ovf;
	uint8_t				fifo_full;
	uint32_t			num_bytes;
	uint8_t				rx_not_empty;
	uint8_t				rx_status;
	char			*	rx_fifo;
	uint32_t			fifo_size;	
}UART_FIFO;

extern UART_FIFO uart0, uart1, uart2, uart3;

extern void 	uart_init(uint8_t, uint32_t);
extern void 	uart_init_fifo(uint8_t,uint32_t);
extern char 	uart_getc(uint8_t);
extern void		uart_putc(uint8_t, char);
extern void 	uart_puts(uint8_t port, char  *, uint32_t);
extern void 	uart_print(uint8_t port, char  *);

#endif