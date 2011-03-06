
#include "usart.h"
#include "my_typedef.h"
#include "common.h"
#include <avr/io.h>
#include <util/delay.h>

BOOL Usart_Wait_WriteComplete( VOID )
{
#ifdef CO_USART_TX_INTERRUPT
	while( gUsart_TxBuffer.head != gUsart_TxBuffer.tail ) {
		;
	}
#endif

	while( !( D_UCSRA & _BV(D_UDRE) ) ) {
		;
	}

	// システムクロックによっては間に合わないかもしれない
	sbi( D_UCSRA, TXC0 );
	while( !( D_UCSRA & _BV(TXC0) ) ) {
		;
	}

	return TRUE;
}

