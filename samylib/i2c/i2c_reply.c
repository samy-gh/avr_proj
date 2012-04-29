
// I2C
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID i2c_reply( UCHAR ack )
{
	// transmit master read ready signal, with or without ack
	if( ack ) {
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT) | _BV(TWEA);
	}
	else {
		TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWINT);
	}
}

