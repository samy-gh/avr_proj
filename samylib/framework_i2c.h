
#ifndef __FRAMEWORK_I2C_H__
#define __FRAMEWORK_I2C_H__

// samylib
#include "my_typedef.h"

extern volatile UCHAR _gui_event_i2c_slave_read;

// äOïîä÷êî
extern VOID i2c_slave_read_event_init( VOID );
extern VOID i2c_slave_read_event_finish( VOID );
extern VOID i2c_slave_read_event_finish( VOID );
extern VOID i2c_slave_read_event_get_buf( UCHAR** const buf_addr, UINT* const buf_len );


#endif //__FRAMEWORK_I2C_H__

