
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_Read_Master( const UCHAR address, UCHAR* const data, UCHAR length )
{
	UCHAR i;

	// ensure data will fit into buffer
	if( I2C_BUFFER_LENGTH < length ) {
		return 0;
	}

	// wait until twi is ready, become master receiver
	while( TWI_READY != i2c_state ) {
		continue;
	}
	i2c_state = TWI_MRX;
	// reset error state (0xFF.. no error occured)
	i2c_error = 0xFF;

	// initialize buffer iteration vars
	i2c_masterBufferIndex = 0;
	i2c_masterBufferLength = length-1;  // This is not intuitive, read on...
	// On receive, the previously configured ACK/NACK setting is transmitted in
	// response to the received byte before the interrupt is signalled.
	// Therefor we must actually set NACK when the _next_ to last byte is
	// received, causing that NACK to be sent in response to receiving the last
	// expected byte of data.

	// build sla+w, slave device address + w bit
	i2c_slarw = TW_READ;
	i2c_slarw |= address << 1;

	// send start condition
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

	// wait for read operation to complete
	while( TWI_MRX == i2c_state ) {
		continue;
	}

	if( i2c_masterBufferIndex < length ) {
		length = i2c_masterBufferIndex;
	}

	// copy twi buffer to data
	for( i = 0; i < length; ++i ) {
		data[i] = i2c_masterBuffer[i];
	}

	return length;
}

