
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


UCHAR Usart_Available( VOID )
{
	return (D_USART_RX_BUFFER_SIZE + gUsart_RxBuffer.head - gUsart_RxBuffer.tail) % D_USART_RX_BUFFER_SIZE;
}

