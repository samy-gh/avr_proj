
#include "usart.h"
#include "my_typedef.h"
#include <avr/io.h>
#include "avr/sleep.h"
#include <avr/interrupt.h>

#if defined(USART_RX_vect)
SIGNAL(USART_RX_vect)
{

	UCHAR c = D_UDR;

	UINT i = (gUsart_RxBuffer.head + 1) % D_USART_RX_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != gUsart_RxBuffer.tail) {
		gUsart_RxBuffer.buffer[gUsart_RxBuffer.head] = c;
		gUsart_RxBuffer.head = i;
	}

	sleep_disable();
}
#elif defined(USART0_RX_vect)
SIGNAL(USART0_RX_vect)
{

	UCHAR c = D_UDR;

	UINT i = (gUsart_RxBuffer.head + 1) % D_USART_RX_BUFFER_SIZE;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != gUsart_RxBuffer.tail) {
		gUsart_RxBuffer.buffer[gUsart_RxBuffer.head] = c;
		gUsart_RxBuffer.head = i;
	}

	sleep_disable();
}

#else
  #error No interrupt handler for usart 0
#endif

