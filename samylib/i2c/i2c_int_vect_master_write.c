
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_int_hdl_master_write( VOID )
{
	switch( TW_STATUS ) {
		// Master Transmitter
		case TW_MT_SLA_ACK:  // slave receiver acked address
		case TW_MT_DATA_ACK: // slave receiver acked data
			// if there is data to send, send it, otherwise stop 
			if(i2c_masterBufferIndex < i2c_masterBufferLength){
				// copy data to output register and ack
				TWDR = i2c_masterBuffer[i2c_masterBufferIndex++];
				i2c_reply(1);
			}else{
				i2c_stop();
				if( i2c_onMasterEnd ) {
				  i2c_onMasterEnd( i2c_error );
				}
			}
			return 1;
		case TW_MT_SLA_NACK:  // address sent, nack received
			i2c_error = TW_MT_SLA_NACK;
			i2c_stop();
			if( i2c_onMasterEnd ) {
				i2c_onMasterEnd( i2c_error );
			}
			return 1;
		case TW_MT_DATA_NACK: // data sent, nack received
			i2c_error = TW_MT_DATA_NACK;
			i2c_stop();
			if( i2c_onMasterEnd ) {
				i2c_onMasterEnd( i2c_error );
			}
			return 1;
		case TW_MT_ARB_LOST: // lost bus arbitration
			i2c_error = TW_MT_ARB_LOST;
			i2c_Release_Bus();
			if( i2c_onMasterEnd ) {
				i2c_onMasterEnd( i2c_error );
			}
			return 1;
	}

	return 0;
}

