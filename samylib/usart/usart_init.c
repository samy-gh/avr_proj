
#include "usart.h"
#include "my_typedef.h"
#include "common.h"
#include <avr/io.h>


volatile T_USART_RING_BUF_RX gUsart_RxBuffer = { {0}, 0, 0 };
#ifdef CO_USART_TX_INTERRUPT
volatile T_USART_RING_BUF_TX gUsart_TxBuffer = { {0}, 0, 0 };
#endif //CO_USART_TX_INTERRUPT


VOID Usart_Init( const ULONG baud )
{
	cbi( PRR, PRUSART0 );		// Disable power save mode

	D_UBRRH = ((F_CPU / 16 + baud / 2) / baud - 1) >> 8;
	D_UBRRL = ((F_CPU / 16 + baud / 2) / baud - 1);

	// enable rx and tx
	sbi( D_UCSRB, D_RXEN );
	sbi( D_UCSRB, D_TXEN );

	// enable interrupt on complete reception of a byte
	sbi( D_UCSRB, D_RXCIE );
	// defaults to 8-bit, no parity, 1 stop bit
}

