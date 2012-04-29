
// I2C
#include "i2c.h"

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/eeprom.h>


UCHAR i2c_Set_SlaveAddress_FromEeprom( const UINT eeprom_addr )
{
	uint8_t i2c_addr = eeprom_read_byte( (unsigned char *) eeprom_addr );

	i2c_Set_SlaveAddress( i2c_addr );

	return i2c_addr;
}

