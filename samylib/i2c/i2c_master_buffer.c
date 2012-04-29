
// I2C
#include "i2c/i2c_local.h"

// samylib
#include "my_typedef.h"


UCHAR i2c_masterBuffer[I2C_BUFFER_LENGTH];
volatile UCHAR i2c_masterBufferIndex;
UCHAR i2c_masterBufferLength;

UCHAR i2c_slarw;

VOID (*i2c_onMasterEnd)( UCHAR );

