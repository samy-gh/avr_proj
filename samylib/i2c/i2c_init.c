
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>


VOID i2c_init( const ULONG i2c_freq )
{
	// initialize state
	i2c_state = TWI_READY;

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
	// activate internal pull-ups for twi
	// as per note from atmega8 manual pg167
	sbi( PORTC, 4 );
	sbi( PORTC, 5 );
#else
	// activate internal pull-ups for twi
	// as per note from atmega128 manual pg204
	sbi( PORTD, 0 );
	sbi( PORTD, 1 );
#endif

	// initialize twi prescaler and bit rate
	cbi( TWSR, TWPS0 );
	cbi( TWSR, TWPS1 );
	TWBR = ((F_CPU / i2c_freq) - 16) / 2;

	/* twi bit rate formula from atmega128 manual pg 204
	SCL Frequency = CPU Clock Frequency / (16 + (2 * TWBR))
	note: TWBR should be 10 or higher for master mode
	It is 72 for a 16mhz Wiring board with 100kHz TWI */

	// enable twi module, acks, and twi interrupt
	TWCR = _BV(TWEN) | _BV(TWIE) | _BV(TWEA);
}

