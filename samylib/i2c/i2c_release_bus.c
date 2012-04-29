
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


/* 
 * Function twi_Release_Bus
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
VOID i2c_Release_Bus( VOID )
{
	// release bus
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA) | _BV(TWINT);

	// update twi state
	i2c_state = TWI_READY;
}

