
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_Write_Master( const UCHAR address, const UCHAR* const data, const UCHAR length, const UCHAR wait )
{
	UCHAR i;

	// ensure data will fit into buffer
	if( I2C_BUFFER_LENGTH < length ) {
		return 1;
	}

	// wait until twi is ready, become master transmitter
	while( TWI_READY != i2c_state ) {
		continue;
	}
	i2c_state = TWI_MTX;
	// reset error state (0xFF.. no error occured)
	i2c_error = 0xFF;

	// initialize buffer iteration vars
	i2c_masterBufferIndex = 0;
	i2c_masterBufferLength = length;

	// copy data to twi buffer
	for( i = 0; i < length; ++i ) {
		i2c_masterBuffer[i] = data[i];
	}

	// build sla+w, slave device address + w bit
	i2c_slarw = TW_WRITE;
	i2c_slarw |= address << 1;

	// send start condition
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWSTA);

	// wait for write operation to complete
	while( wait && (TWI_MTX == i2c_state) ) {
		continue;
	}

	if( i2c_error == 0xFF ) {
		return 0;	// success
	}
	else if( i2c_error == TW_MT_SLA_NACK ) {
		return 2;	// error: address send, nack received
	}
	else if( i2c_error == TW_MT_DATA_NACK ) {
		return 3;	// error: data send, nack received
	}
	else {
		return 4;	// other twi error
	}
}

