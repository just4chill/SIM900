
#include "uart.h"
#include "LPC17xx.h"
#include "system_LPC17xx.h"
#include <stdint.h>
#include "string.h"
#include "FreeRTOS.h"

UART_FIFO uart0, uart1, uart2, uart3;

void UART0_IRQHandler(void)
{
	uint8_t IIRValue, LSRValue;
	IIRValue 		= LPC_UART0->IIR;
	IIRValue 		>>= 1; /* skip pending bit in IIR */
	IIRValue	 	&= 0x07; /* check bit 1~3, interrupt identification */
	char c 			= '\0';

	// If RLS
	if ( IIRValue == IIR_RLS )
	{
		LSRValue = LPC_UART0->LSR;

		/* There are errors or break interrupt */
		/* Read LSR will clear the interrupt */
		if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
			uart0.rx_status = LSRValue;
			// Read to clear interrupt
			char dummy = LPC_UART0->RBR;
			return;
		}

		/* If no error on RLS, normal ready, save into the data buffer. */
		/* Note: read RBR will clear the interrupt */
		if ( LSRValue & LSR_RDR )
		{
			c = LPC_UART0->RBR;
		}
	}

	/* Receive Data Available */
	else if ( IIRValue == IIR_RDA )
	{
		c = LPC_UART0->RBR;
	}
	// Timeout
 	else if ( IIRValue == IIR_CTI ) 	/* Character timeout indicator */
	{
		/* Character Time-out indicator */
		uart0.rx_status |= 0x100; 		/* Bit 9 as the CTI error */
	}

	if(c != '\0')
	{
		if(uart0.num_bytes == uart0.fifo_size)
		{
			uart0.rx_ovf = 1;
		}
		else if(uart0.num_bytes < uart0.fifo_size)
		{
			uart0.rx_fifo[uart0.i_last] = c;
			uart0.i_last++;
			uart0.num_bytes++;
		}
		if(uart0.num_bytes == uart0.fifo_size)
		{
			uart0.fifo_full = 1;
		}
		if(uart0.i_last == uart0.fifo_size)
		{
			uart0.i_last = 0;
		}
		uart0.rx_not_empty = 1;
	}
}

void UART1_IRQHandler(void)
{
	uint8_t IIRValue, LSRValue;
	IIRValue 		= LPC_UART3->IIR;
	IIRValue 		>>= 1; /* skip pending bit in IIR */
	IIRValue	 	&= 0x07; /* check bit 1~3, interrupt identification */
	char c 			= '\0';

	// If RLS
	if ( IIRValue == IIR_RLS )
	{
		LSRValue = LPC_UART1->LSR;

		/* There are errors or break interrupt */
		/* Read LSR will clear the interrupt */
		if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
			uart1.rx_status = LSRValue;
			// Read to clear interrupt
			char dummy = LPC_UART1->RBR;
			return;
		}

		/* If no error on RLS, normal ready, save into the data buffer. */
		/* Note: read RBR will clear the interrupt */
		if ( LSRValue & LSR_RDR )
		{
			c = LPC_UART1->RBR;
		}
	}

	/* Receive Data Available */
	else if ( IIRValue == IIR_RDA )
	{
		c = LPC_UART1->RBR;
	}
	// Timeout
 	else if ( IIRValue == IIR_CTI ) 	/* Character timeout indicator */
	{
		/* Character Time-out indicator */
		uart1.rx_status |= 0x100; 		/* Bit 9 as the CTI error */
	}

	if(c != '\0')
	{
		if(uart1.num_bytes == uart1.fifo_size)
		{
			uart1.rx_ovf = 1;
		}
		else if(uart1.num_bytes < uart1.fifo_size)
		{
			uart1.rx_fifo[uart1.i_last] = c;
			uart1.i_last++;
			uart1.num_bytes++;
		}
		if(uart1.num_bytes == uart1.fifo_size)
		{
			uart1.fifo_full = 1;
		}
		if(uart1.i_last == uart1.fifo_size)
		{
			uart1.i_last = 0;
		}
		uart1.rx_not_empty = 1;
	}
}

void UART2_IRQHandler(void)
{
	uint8_t IIRValue, LSRValue;
	IIRValue 		= LPC_UART2->IIR;
	IIRValue 		>>= 1; /* skip pending bit in IIR */
	IIRValue	 	&= 0x07; /* check bit 1~3, interrupt identification */
	char c 			= '\0';

	// If RLS
	if ( IIRValue == IIR_RLS )
	{
		LSRValue = LPC_UART2->LSR;

		/* There are errors or break interrupt */
		/* Read LSR will clear the interrupt */
		if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
			uart2.rx_status = LSRValue;
			// Read to clear interrupt
			char dummy = LPC_UART2->RBR;
			return;
		}

		/* If no error on RLS, normal ready, save into the data buffer. */
		/* Note: read RBR will clear the interrupt */
		if ( LSRValue & LSR_RDR )
		{
			c = LPC_UART2->RBR;
		}
	}

	/* Receive Data Available */
	else if ( IIRValue == IIR_RDA )
	{
		c = LPC_UART2->RBR;
	}
	// Timeout
 	else if ( IIRValue == IIR_CTI ) 	/* Character timeout indicator */
	{
		/* Character Time-out indicator */
		uart2.rx_status |= 0x100; 		/* Bit 9 as the CTI error */
	}

	if(c != '\0')
	{
		if(uart2.num_bytes == uart2.fifo_size)
		{
			uart2.rx_ovf = 1;
		}
		else if(uart2.num_bytes < uart2.fifo_size)
		{
			uart2.rx_fifo[uart2.i_last] = c;
			uart2.i_last++;
			uart2.num_bytes++;
		}
		if(uart2.num_bytes == uart2.fifo_size)
		{
			uart2.fifo_full = 1;
		}
		if(uart2.i_last == uart2.fifo_size)
		{
			uart2.i_last = 0;
		}
		uart2.rx_not_empty = 1;
	}
}

void UART3_IRQHandler(void)
{
	uint8_t IIRValue, LSRValue;
	IIRValue 		= LPC_UART3->IIR;
	IIRValue 		>>= 1; /* skip pending bit in IIR */
	IIRValue	 	&= 0x07; /* check bit 1~3, interrupt identification */
	char c 			= '\0';

	// If RLS
	if ( IIRValue == IIR_RLS )
	{
		LSRValue = LPC_UART3->LSR;

		/* There are errors or break interrupt */
		/* Read LSR will clear the interrupt */
		if ( LSRValue & (LSR_OE|LSR_PE|LSR_FE|LSR_RXFE|LSR_BI) )
		{
			uart3.rx_status = LSRValue;
			// Read to clear interrupt
			char dummy = LPC_UART3->RBR;
			return;
		}

		/* If no error on RLS, normal ready, save into the data buffer. */
		/* Note: read RBR will clear the interrupt */
		if ( LSRValue & LSR_RDR )
		{
			c = LPC_UART3->RBR;
		}
	}

	/* Receive Data Available */
	else if ( IIRValue == IIR_RDA )
	{
		c = LPC_UART3->RBR;
	}
	// Timeout
 	else if ( IIRValue == IIR_CTI ) 	/* Character timeout indicator */
	{
		/* Character Time-out indicator */
		uart3.rx_status |= 0x100; 		/* Bit 9 as the CTI error */
	}

	if(c != '\0')
	{
		if(uart3.num_bytes == uart3.fifo_size)
		{
			uart3.rx_ovf = 1;
		}
		else if(uart3.num_bytes < uart3.fifo_size)
		{
			uart3.rx_fifo[uart3.i_last] = c;
			uart3.i_last++;
			uart3.num_bytes++;
		}
		if(uart3.num_bytes == uart3.fifo_size)
		{
			uart3.fifo_full = 1;
		}
		if(uart3.i_last == uart3.fifo_size)
		{
			uart3.i_last = 0;
		}
		uart3.rx_not_empty = 1;
	}
}

char uart_getc(uint8_t _port)
{
	if(_port == 0)
	{
		char _byte = '\0';
		if(uart0.num_bytes == uart0.fifo_size)
		{
			uart0.fifo_full = 0;
		}

		if(uart0.num_bytes > 0)
		{
			_byte = uart0.rx_fifo[uart0.i_first];
			uart0.i_first++;
			uart0.num_bytes--;
		}
		else
		{
			uart0.rx_not_empty = 0;
		}

		if(uart0.i_first == uart0.fifo_size)
		{
			uart0.i_first = 0;
		}

		return _byte;		
	}

	else if(_port == 1)
	{
		char _byte = '\0';
		if(uart1.num_bytes == uart1.fifo_size)
		{
			uart1.fifo_full = 0;
		}

		if(uart1.num_bytes > 0)
		{
			_byte = uart1.rx_fifo[uart1.i_first];
			uart1.i_first++;
			uart1.num_bytes--;
		}
		else
		{
			uart1.rx_not_empty = 0;
		}

		if(uart1.i_first == uart1.fifo_size)
		{
			uart1.i_first = 0;
		}

		return _byte;		
	}

	else if(_port == 2)
	{
		char _byte = '\0';
		if(uart2.num_bytes == uart2.fifo_size)
		{
			uart2.fifo_full = 0;
		}

		if(uart2.num_bytes > 0)
		{
			_byte = uart2.rx_fifo[uart2.i_first];
			uart2.i_first++;
			uart2.num_bytes--;
		}
		else
		{
			uart2.rx_not_empty = 0;
		}

		if(uart2.i_first == uart2.fifo_size)
		{
			uart2.i_first = 0;
		}

		return _byte;		
	}

	else if(_port == 3)
	{
		char _byte = '\0';
		if(uart3.num_bytes == uart3.fifo_size)
		{
			uart3.fifo_full = 0;
		}

		if(uart3.num_bytes > 0)
		{
			_byte = uart3.rx_fifo[uart3.i_first];
			uart3.i_first++;
			uart3.num_bytes--;
		}
		else
		{
			uart3.rx_not_empty = 0;
		}

		if(uart3.i_first == uart3.fifo_size)
		{
			uart3.i_first = 0;
		}

		return _byte;		
	}
	
	// Never execute
	return 0;
}


void uart_init(uint8_t port, uint32_t baudrate)
{
	uint32_t Fdiv;
	uint32_t pclkdiv, pclk;

	if(port == 0)
		pclkdiv = (LPC_SC->PCLKSEL0 >> 6) & 0x03;
	else if(port == 1)
		pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
	// TODO: For UART2
	else if(port == 2)
		pclkdiv = (LPC_SC->PCLKSEL0 >> 8) & 0x03;
	else if(port == 3)
		pclkdiv = (LPC_SC->PCLKSEL1 >> 18) & 0x03;

	switch ( pclkdiv )
	{
		case 0x00:
		default:
			pclk = SystemCoreClock / 4;
			break;
		case 0x01:
			pclk = SystemCoreClock;
			break;
		case 0x02:
			pclk = SystemCoreClock / 2;
			break;
		case 0x03:
			pclk = SystemCoreClock / 8;
			break;
	}

	if(port == 0)
	{
		LPC_PINCON->PINSEL0 &= ~0x000000F0;
		LPC_PINCON->PINSEL0 |= 0x00000050; /* RxD0 is P0.3 and TxD0 is P0.2 */

		LPC_UART0->LCR 			= 0x83; /* 8 bits, no Parity, 1 Stop bit */
		Fdiv 					= ( pclk / 16 ) / baudrate ; /*baud rate */

		LPC_UART0->DLM 			= Fdiv / 256;
		LPC_UART0->DLL 			= Fdiv % 256;

		LPC_UART0->LCR 			= 0x03; /* DLAB = 0 */
		LPC_UART0->FCR 			= 0x07; /* Enable and reset TX and RX FIFO. */

		NVIC_EnableIRQ(UART0_IRQn);
		LPC_UART0->IER = IER_RBR | IER_RLS; /* Enable UART0 interrupt */
	}	

	else if(port == 1)
	{
		LPC_PINCON->PINSEL4 &= ~0x0000000F;
		LPC_PINCON->PINSEL4 |= 0x0000000A; /* Enable RxD1 P2.1, TxD1 P2.0 */

		LPC_UART1->LCR 			= 0x83; /* 8 bits, no Parity, 1 Stop bit */
		Fdiv 					= ( pclk / 16 ) / baudrate ; /*baud rate */

		LPC_UART1->DLM 			= Fdiv / 256;
		LPC_UART1->DLL 			= Fdiv % 256;

		LPC_UART1->LCR 			= 0x03; /* DLAB = 0 */
		LPC_UART1->FCR 			= 0x07; /* Enable and reset TX and RX FIFO. */

		NVIC_EnableIRQ(UART1_IRQn);
		LPC_UART1->IER = IER_RBR | IER_RLS; /* Enable UART0 interrupt */
	}

	// TODO: UART2
	else if(port == 2)
	{
		LPC_PINCON->PINSEL4 &= ~0x0000000F;
		LPC_PINCON->PINSEL4 |= 0x0000000A; /* Enable RxD1 P2.1, TxD1 P2.0 */

		LPC_UART2->LCR 			= 0x83; /* 8 bits, no Parity, 1 Stop bit */
		Fdiv 					= ( pclk / 16 ) / baudrate ; /*baud rate */

		LPC_UART2->DLM 			= Fdiv / 256;
		LPC_UART2->DLL 			= Fdiv % 256;

		LPC_UART2->LCR 			= 0x03; /* DLAB = 0 */
		LPC_UART2->FCR 			= 0x07; /* Enable and reset TX and RX FIFO. */

		NVIC_EnableIRQ(UART2_IRQn);
		LPC_UART2->IER = IER_RBR | IER_RLS; /* Enable UART0 interrupt */
	}

	else if(port == 3)
	{
		// Turn on UART2 peripheral clock
		LPC_SC->PCLKSEL1 &= ~(3 << 18);
		LPC_SC->PCLKSEL1 |=  (0 << 18);		// PCLK_periph = CCLK/4

		LPC_SC->PCONP |= (1 << 25);

		// Set PINSEL0 so that P0.0 = TXD3, P0.1 = RXD3
		LPC_PINCON->PINSEL0 &= ~0xf;
		LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3));

		LPC_UART3->LCR 				= 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	    Fdiv 						= ( pclk / 16 ) / baudrate ;	// Set baud rate
	    LPC_UART3->DLM 				= Fdiv / 256;
	    LPC_UART3->DLL 				= Fdiv % 256;
	    LPC_UART3->ACR 				= 0x00;		// Disable autobaud
	    LPC_UART3->LCR 				= 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	    LPC_UART3->FCR 				= 0x07;		// Enable and reset TX and RX FIFO

	    NVIC_EnableIRQ(UART3_IRQn);
		LPC_UART3->IER = IER_RBR | IER_RLS; /* Enable UART3 interrupt */			
	}
}


void uart_init_fifo(uint8_t _port, uint32_t _size)
{
	if(_port == 0)
	{
		uart0.i_first = 0;			
	 	uart0.i_last = 0;				
	 	uart0.rx_ovf = 0;
	 	uart0.fifo_full = 0;
	 	uart0.num_bytes = 0;
	 	uart0.rx_not_empty = 0;
	 	uart0.rx_fifo = (char *) pvPortMalloc(_size);	
	 	uart0.fifo_size	= _size;	
	}
	else if(_port == 1)
	{
		uart1.i_first = 0;			
	 	uart1.i_last = 0;				
	 	uart1.rx_ovf = 0;
	 	uart1.fifo_full = 0;
	 	uart1.num_bytes = 0;
	 	uart1.rx_not_empty = 0;
	 	uart1.rx_fifo = (char *) pvPortMalloc(_size);	
	 	uart1.fifo_size	= _size;	
	}
	else if(_port == 2)
	{
		uart2.i_first = 0;			
	 	uart2.i_last = 0;				
	 	uart2.rx_ovf = 0;
	 	uart2.fifo_full = 0;
	 	uart2.num_bytes = 0;
	 	uart2.rx_not_empty = 0;
	 	uart2.rx_fifo = (char *) pvPortMalloc(_size);	
	 	uart2.fifo_size	= _size;
	}
	else if(_port == 3)
	{
		uart3.i_first = 0;			
	 	uart3.i_last = 0;				
	 	uart3.rx_ovf = 0;
	 	uart3.fifo_full = 0;
	 	uart3.num_bytes = 0;
	 	uart3.rx_not_empty = 0;
	 	uart3.rx_fifo = (char *) pvPortMalloc(_size);	
	 	uart3.fifo_size	= _size;
	}
}

void uart_putc(uint8_t port, char c)
{
	if(port == 0)
	{
		LPC_UART0->THR = c;
		while(!((LPC_UART0->LSR) & TEMT));
	}
	else if(port == 1)
	{
		LPC_UART1->THR = c;
		while(!((LPC_UART1->LSR) & TEMT));		
	}
	else if(port == 2)
	{
		LPC_UART2->THR = c;
		while(!((LPC_UART2->LSR) & TEMT));		
	}
	else if(port == 3)
	{
		LPC_UART3->THR = c;
		while(!((LPC_UART3->LSR) & TEMT));		
	}
}

void uart_puts(uint8_t port, char  * ptr, uint32_t len)
{
	if(port == 0)
		for(uint8_t i = 0;i < len;uart_putc(0, ptr[i++]));
	else if(port == 1)
		for(uint8_t i = 0;i < len;uart_putc(1, ptr[i++]));
	else if(port == 2)
		for(uint8_t i = 0;i < len;uart_putc(2, ptr[i++]));	
	else if(port == 3)
		for(uint8_t i = 0;i < len;uart_putc(3, ptr[i++]));
}

void uart_print(uint8_t port, char  * ptr)
{
	if(port == 0)
		for(;*ptr != '\0';uart_putc(0, *(ptr++)));
	else if(port == 1)
		for(;*ptr != '\0';uart_putc(1, *(ptr++)));
	else if(port == 2)
		for(;*ptr != '\0';uart_putc(2, *(ptr++)));	
	else if(port == 3)
		for(;*ptr != '\0';uart_putc(3, *(ptr++)));
}

uint32_t uart_readline(uint8_t port, char * line)
{
		char c = '\0';
		uint32_t i = 0;
		if(port == 0)
		{
			while(c != '\n')
			{	
				if(uart0.num_bytes > 0)
				{
					c = uart_getc(0);
					line[i++] = c;
				}
				if(c == '\n')
						break;
			}
			line[i] = '\0';
		}

		else if(port == 1)
		{
			while(c != '\n')
			{	
				if(uart1.num_bytes > 0)
				{
					c = uart_getc(1);
					line[i++] = c;
				}
				if(c == '\n')
					break;
			}
			line[i] = '\0';
		}

		else if(port == 2)
		{
			while(c != '\n')
			{	
				if(uart2.num_bytes > 0)
				{
					c = uart_getc(2);
					line[i++] = c;
				}
				if(c == '\n')
					break;
			}
			line[i] = '\0';
		}

		else if(port == 3)
		{
			while(c != '\r')
			{	
				if(uart3.num_bytes > 0)
				{
					c = uart_getc(3);
					line[i++] = c;
				}
				if(c == '\r')
					break;
			}
			line[i] = '\0';
		}
		return i;
}