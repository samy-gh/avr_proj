
#include "usart.h"
#include "my_typedef.h"
#include "common.h"
#include <avr/io.h>

#ifdef CO_USART_TX_INTERRUPT
inline static VOID usart_store_char_tx( const UCHAR c )
{
	while( 1 ) {
		UINT i = (gUsart_TxBuffer.head + 1) % D_USART_TX_BUFFER_SIZE;

		// if we should be storing the received character into the location
		// just before the tail (meaning that the head would advance to the
		// current location of the tail), we're about to overflow the buffer
		// and so we don't write the character or advance the head.
		if (i != gUsart_TxBuffer.tail) {
			gUsart_TxBuffer.buffer[gUsart_TxBuffer.head] = c;
			gUsart_TxBuffer.head = i;
			break;
		}
	}
}

VOID Usart_Write( const UCHAR c )
{
	usart_store_char_tx( c );

	if( (D_UCSRA & _BV(D_UDRE)) ) {
		sbi( D_UCSRB, D_UDRIE );
	}
}
#else //!CO_USART_TX_INTERRUPT
VOID Usart_Write( const UCHAR c )
{
	while( !( D_UCSRA & (1 << D_UDRE) ) ) {
		;
	}

	UDR0 = c;
}
#endif //!CO_USART_TX_INTERRUPT

