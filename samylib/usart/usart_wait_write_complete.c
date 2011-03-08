
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <util/delay.h>


BOOL Usart_Wait_WriteComplete( VOID )
{
#ifdef CO_USART_TX_INTERRUPT
	while( gUsart_TxBuffer.head != gUsart_TxBuffer.tail ) {
		;
	}
#endif

	loop_until_bit_is_set( D_UCSRA, D_UDRE );

	// システムクロックによっては間に合わないかもしれない
	sbi( D_UCSRA, TXC0 );

	loop_until_bit_is_set( D_UCSRA, TXC0 );

	return TRUE;
}

