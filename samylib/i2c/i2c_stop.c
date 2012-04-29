
// I2C
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID i2c_stop( VOID )
{
	// send stop condition
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTO);

	// wait for stop condition to be exectued on bus
	// TWINT is not set after a stop condition!
	while( TWCR & _BV(TWSTO) ) {
		continue;
	}

	// update twi state
	i2c_state = TWI_READY;
}

