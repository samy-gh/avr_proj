
#ifndef __MY_STDOUT_H__
#define __MY_STDOUT_H__

// samylib
#include "my_typedef.h"

// WinAVR
#include <avr/pgmspace.h>


VOID Mystdout_PrintDigitN( UINT val, UINT print_lvl );
VOID Mystdout_PrintDigit( UINT val );
VOID Mystdout_PrintHexN( UINT val, UINT print_lvl );
VOID Mystdout_PrintHex( UINT val );
VOID Mystdout_Putc( const CHAR c );
VOID Mystdout_Puts( const CHAR* str );
VOID Mystdout_PgmPuts( PGM_P p_str );

#endif /* __MY_STDOUT_H__ */
