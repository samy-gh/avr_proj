
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/io.h>


VOID i2c_Attach_SlaveTxEvent( VOID (*function)(VOID) )
{
	i2c_onSlaveTransmit = function;
}

