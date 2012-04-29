
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_SlaveRxBuffer[I2C_BUFFER_LENGTH];
volatile UCHAR i2c_SlaveRxBufferIndex;
VOID (*i2c_onSlaveReceive)( UCHAR*, INT );


UCHAR i2c_int_hdl_slave_read( VOID )
{
	switch( TW_STATUS ) {
		// Slave Receiver
		case TW_SR_SLA_ACK:   // addressed, returned ack
		case TW_SR_GCALL_ACK: // addressed generally, returned ack
		case TW_SR_ARB_LOST_SLA_ACK:   // lost arbitration, returned ack
		case TW_SR_ARB_LOST_GCALL_ACK: // lost arbitration, returned ack
			// enter slave receiver mode
			i2c_state = TWI_SRX;
			// indicate that rx buffer can be overwritten and ack
			i2c_SlaveRxBufferIndex = 0;
			i2c_reply( 1 );
			return 1;
		case TW_SR_DATA_ACK:       // data received, returned ack
		case TW_SR_GCALL_DATA_ACK: // data received generally, returned ack
			// if there is still room in the rx buffer
			if( i2c_SlaveRxBufferIndex < I2C_BUFFER_LENGTH ) {
				// put byte in buffer and ack
				i2c_SlaveRxBuffer[i2c_SlaveRxBufferIndex++] = TWDR;
				i2c_reply( 1 );
			}else{
				// otherwise nack
				i2c_reply( 0 );
			}
			return 1;
		case TW_SR_STOP: // stop or repeated start condition received
			// put a null char after data if there's room
			if( i2c_SlaveRxBufferIndex < I2C_BUFFER_LENGTH ) {
				i2c_SlaveRxBuffer[i2c_SlaveRxBufferIndex] = '\0';
			}
			// sends ack and stops interface for clock stretching
			i2c_stop();
			// callback to user defined callback
			if( i2c_onSlaveReceive ) {
				i2c_onSlaveReceive( i2c_SlaveRxBuffer, i2c_SlaveRxBufferIndex );
			}
			// since we submit rx buffer to "wire" library, we can reset it
			i2c_SlaveRxBufferIndex = 0;
			// ack future responses and leave slave receiver state
#ifdef CO_TWI_SR_AUTO_RELEASE
			i2c_Release_Bus();
#endif
			return 1;
		case TW_SR_DATA_NACK:       // data received, returned nack
		case TW_SR_GCALL_DATA_NACK: // data received generally, returned nack
			// nack back at master
			i2c_reply( 0 );
			return 1;
	}

	return 0;
}

