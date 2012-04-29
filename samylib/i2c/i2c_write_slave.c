
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


UCHAR i2c_Write_Slave( const UCHAR* const data, const UCHAR length )
{
	UCHAR i;

	// ensure data will fit into buffer
	if( I2C_BUFFER_LENGTH < length ) {
		return 1;
	}

	// ensure we are currently a slave transmitter
	if( TWI_STX != i2c_state ) {
		return 2;
	}

	// set length and copy data into tx buffer
	i2c_SlaveTxBufferLength = length;
	for( i = 0; i < length; ++i ) {
		i2c_SlaveTxBuffer[i] = data[i];
	}

	return 0;
}

