
// I2C
#include "i2c.h"
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"
#include "common.h"

// WinAVR
#include <avr/io.h>
#include <compat/twi.h>


UCHAR i2c_SlaveTxBuffer[I2C_BUFFER_LENGTH];
volatile UCHAR i2c_SlaveTxBufferIndex;
volatile UCHAR i2c_SlaveTxBufferLength;

VOID (*i2c_onSlaveTransmit)( VOID );

UCHAR i2c_int_hdl_slave_write( VOID )
{
	switch( TW_STATUS ) {
		// Slave Transmitter
		case TW_ST_SLA_ACK:          // addressed, returned ack
		case TW_ST_ARB_LOST_SLA_ACK: // arbitration lost, returned ack
			// enter slave transmitter mode
			i2c_state = TWI_STX;
			// ready the tx buffer index for iteration
			i2c_SlaveTxBufferIndex = 0;
			// set tx buffer length to be zero, to verify if user changes it
			i2c_SlaveTxBufferLength = 0;
			// request for txBuffer to be filled and length to be set
			// note: user must call i2c_transmit(bytes, length) to do this
			if( i2c_onSlaveTransmit ) {
				i2c_onSlaveTransmit();
			}
			// if they didn't change buffer & length, initialize it
			if( 0 == i2c_SlaveTxBufferLength ) {
				i2c_SlaveTxBufferLength = 1;
				i2c_SlaveTxBuffer[0] = 0x00;
			}
			// transmit first byte from buffer, fall
		case TW_ST_DATA_ACK: // byte sent, ack returned
			// copy data to output register
			TWDR = i2c_SlaveTxBuffer[i2c_SlaveTxBufferIndex++];
			// if there is more to send, ack, otherwise nack
			if( i2c_SlaveTxBufferIndex < i2c_SlaveTxBufferLength ) {
				i2c_reply( 1 );
			}
			else {
			  i2c_reply( 0 );
			}
			return 1;
		case TW_ST_DATA_NACK: // received nack, we are done 
		case TW_ST_LAST_DATA: // received ack, but we are done already!
			// ack future responses
			i2c_reply( 1 );
			// leave slave receiver state
			i2c_state = TWI_READY;
			return 1;
	}
	return 0;
}

