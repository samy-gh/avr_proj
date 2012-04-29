
#ifndef __I2C_LOCAL_H__
#define __I2C_LOCAL_H__

// I2C
#include "i2c_custom.h"

// samylib
#include "my_typedef.h"


// Driver status
#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4


// Master buffer
extern UCHAR i2c_masterBuffer[I2C_BUFFER_LENGTH];
extern volatile UCHAR i2c_masterBufferIndex;
extern UCHAR i2c_masterBufferLength;
extern UCHAR i2c_slarw;

extern UCHAR i2c_SlaveTxBuffer[I2C_BUFFER_LENGTH];
extern volatile UCHAR i2c_SlaveTxBufferIndex;
extern volatile UCHAR i2c_SlaveTxBufferLength;
extern VOID (*i2c_onSlaveTransmit)( VOID );

extern UCHAR i2c_SlaveRxBuffer[I2C_BUFFER_LENGTH];
extern volatile UCHAR i2c_SlaveRxBufferIndex;
extern VOID (*i2c_onSlaveReceive)( UCHAR*, INT );

extern volatile UCHAR i2c_state;
extern volatile UCHAR i2c_error;

// Int handler function pointer
extern UCHAR (*i2c_func_master_write_hdl)( VOID );
extern UCHAR (*i2c_func_master_read_hdl)( VOID );
extern UCHAR (*i2c_func_slave_write_hdl)( VOID );
extern UCHAR (*i2c_func_slave_read_hdl)( VOID );

// User handler
extern VOID (*i2c_onMasterEnd)( UCHAR );

// internal functions
extern VOID i2c_init( const ULONG i2c_freq );

// Int handler functions
extern UCHAR i2c_int_hdl_master_write( VOID );
extern UCHAR i2c_int_hdl_master_read( VOID );
extern UCHAR i2c_int_hdl_slave_write( VOID );
extern UCHAR i2c_int_hdl_slave_read( VOID );
extern VOID i2c_reply( UCHAR ack );
extern VOID i2c_stop( VOID );


#endif //__I2C_LOCAL_H__

