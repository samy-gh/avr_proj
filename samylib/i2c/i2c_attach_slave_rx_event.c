
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"


VOID i2c_Attach_SlaveRxEvent( VOID (*function)(UCHAR*, INT) )
{
	i2c_onSlaveReceive = function;
}

