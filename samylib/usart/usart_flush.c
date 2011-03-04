
#include "usart.h"
#include "my_typedef.h"

VOID Usart_Flush( VOID )
{
	// don't reverse this or there may be problems if the RX interrupt
	// occurs after reading the value of rx_buffer_head but before writing
	// the value to rx_buffer_tail; the previous value of rx_buffer_head
	// may be written to rx_buffer_tail, making it appear as if the buffer
	// were full, not empty.
	gUsart_RxBuffer.head = gUsart_RxBuffer.tail;
}

