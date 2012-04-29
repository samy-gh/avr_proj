
// I2C
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_int_hdl_master_read( VOID )
{
	switch( TW_STATUS ) {
		// Master Receiver
		case TW_MR_DATA_ACK: // data received, ack sent
			// put byte into buffer
			i2c_masterBuffer[i2c_masterBufferIndex++] = TWDR;
		case TW_MR_SLA_ACK:  // address sent, ack received
			// ack if more bytes are expected, otherwise nack
			if(i2c_masterBufferIndex < i2c_masterBufferLength){
			  i2c_reply(1);
			}else{
			  i2c_reply(0);
			}
			return 1;
		case TW_MR_DATA_NACK: // data received, nack sent
			// put final byte into buffer
			i2c_masterBuffer[i2c_masterBufferIndex++] = TWDR;
		case TW_MR_SLA_NACK: // address sent, nack received
			i2c_stop();
			if( i2c_onMasterEnd ) {
			  i2c_onMasterEnd( i2c_error );
			}
			return 1;
		// TW_MR_ARB_LOST handled by TW_MT_ARB_LOST case
	}

	return 0;
}

