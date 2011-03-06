
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"


INT Usart_Read( VOID )
{
	// if the head isn't ahead of the tail, we don't have any characters
	if (gUsart_RxBuffer.head == gUsart_RxBuffer.tail) {
		return -1;
	} else {
		UCHAR c = gUsart_RxBuffer.buffer[gUsart_RxBuffer.tail];
		gUsart_RxBuffer.tail = (gUsart_RxBuffer.tail + 1) % D_USART_RX_BUFFER_SIZE;
		return c;
	}
}

