
// I2C
#include "i2c.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


VOID i2c_Set_SlaveAddress( const UCHAR address )
{
	// set twi slave address (skip over TWGCE bit)
	TWAR = address << 1;
}

