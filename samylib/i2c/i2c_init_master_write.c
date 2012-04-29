
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID i2c_Init_MasterWrite( const ULONG i2c_freq )
{
	i2c_func_master_write_hdl = i2c_int_hdl_master_write;

	if( TWCR == (_BV(TWEN) | _BV(TWIE) | _BV(TWEA)) ) {
		return;
	}

	i2c_init( i2c_freq );
}

