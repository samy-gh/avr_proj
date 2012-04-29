
#ifndef __I2C_H__
#define __I2C_H__

// samylib
#include "my_typedef.h"


extern VOID i2c_Init_MasterWrite( const ULONG i2c_freq );
extern VOID i2c_Init_MasterRead( const ULONG i2c_freq );
extern VOID i2c_Init_SlaveWrite( const ULONG i2c_freq );
extern VOID i2c_Init_SlaveRead( const ULONG i2c_freq );
extern VOID i2c_Release_Bus( VOID );
extern VOID i2c_Set_SlaveAddress( const UCHAR address );
extern UCHAR i2c_Set_SlaveAddress_FromEeprom( const UINT eeprom_addr );
extern UCHAR i2c_Read_Master( const UCHAR address, UCHAR* const data, UCHAR length );
extern UCHAR i2c_Write_Master( const UCHAR address, const UCHAR* const data, const UCHAR length, const UCHAR wait );
extern UCHAR i2c_Write_Slave( const UCHAR* const data, const UCHAR length );
extern VOID i2c_Attach_SlaveRxEvent( VOID (*function)(UCHAR*, INT) );
extern VOID i2c_Attach_SlaveTxEvent( VOID (*function)(VOID) );
extern VOID i2c_Attach_MasterStopEvent( VOID (*function)(UCHAR) );


#endif /* __I2C_H__ */

