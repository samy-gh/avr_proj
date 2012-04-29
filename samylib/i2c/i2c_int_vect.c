
// I2C
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <compat/twi.h>


UCHAR (*i2c_func_master_write_hdl)( VOID );
UCHAR (*i2c_func_master_read_hdl)( VOID );
UCHAR (*i2c_func_slave_write_hdl)( VOID );
UCHAR (*i2c_func_slave_read_hdl)( VOID );
volatile UCHAR i2c_error;
volatile UCHAR i2c_state;


SIGNAL(TWI_vect)
{
	sleep_disable();


	switch( TW_STATUS ) {
		// All Master
		case TW_START:     // sent start condition
		case TW_REP_START: // sent repeated start condition
			// copy device address and r/w bit to output register and ack
			TWDR = i2c_slarw;
			i2c_reply(1);
			return;
	}

	if( i2c_func_master_write_hdl ) {
		if( i2c_func_master_write_hdl() != 0 ) {
			return;
		}
	}

	if( i2c_func_master_read_hdl ) {
		if( i2c_func_master_read_hdl() != 0 ) {
			return;
		}
	}

	if( i2c_func_slave_write_hdl ) {
		if( i2c_func_slave_write_hdl() != 0 ) {
			return;
		}
	}

	if( i2c_func_slave_read_hdl ) {
		if( i2c_func_slave_read_hdl() != 0 ) {
			return;
		}
	}

	switch( TW_STATUS ) {
		// All
		case TW_NO_INFO:   // no state information
			break;
		case TW_BUS_ERROR: // bus error, illegal stop/start
			i2c_error = TW_BUS_ERROR;
			i2c_stop();
			return;
	}
}

