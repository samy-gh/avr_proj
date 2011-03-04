
#include "usart.h"
#include "my_typedef.h"
#include "common.h"
#include <avr/io.h>
#include "avr/sleep.h"
#include <avr/interrupt.h>

#ifdef CO_USART_TX_INTERRUPT

inline static UINT usart_get_char_tx( UCHAR* const c )
{
	if( gUsart_TxBuffer.head == gUsart_TxBuffer.tail ) {
		return 0;
	}

	*c = gUsart_TxBuffer.buffer[gUsart_TxBuffer.tail];
	gUsart_TxBuffer.tail = (gUsart_TxBuffer.tail + 1) % D_USART_TX_BUFFER_SIZE;

	return 1;
}

#if defined(USART_UDRE_vect)
SIGNAL(USART_UDRE_vect)
{
	UCHAR c;
	if( usart_get_char_tx( &c ) ) {
		D_UDR = c;
	}
	else {
		cbi( D_UCSRB, D_UDRIE );
	}

	sleep_disable();
}
#elif defined(USART0_UDRE_vect)
SIGNAL(USART0_UDRE_vect)
{
	UCHAR c;
	if( usart_get_char_tx( &c ) ) {
		D_UDR = c;
	}
	else {
		cbi( D_UCSRB, D_UDRIE );
	}

	sleep_disable();
}
#else
  #error No interrupt handler for usart 0
#endif

#endif //CO_USART_TX_INTERRUPT

