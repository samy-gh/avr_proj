
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/power.h>


VOID Usart_Close( VOID )
{
	// disable rx and tx
	cbi( D_UCSRB, D_RXEN );
	cbi( D_UCSRB, D_TXEN );

	// disable interrupt on complete reception of a byte
	cbi( D_UCSRB, D_RXCIE );

#ifdef power_usart0_disable
	power_usart0_disable();
#endif
#ifdef power_usart_disable
	power_usart_disable();
#endif
}

