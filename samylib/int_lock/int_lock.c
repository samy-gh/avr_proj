
// samylib
#include "int_lock.h"
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>


UCHAR Int_Lock( VOID )
{
	UCHAR lockKey = SREG;
	sei();

	return lockKey;
}

