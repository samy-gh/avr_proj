
// mystdout
#include "mystdout.h"

// samylib
#include "my_typedef.h"

// STD-C
#include <stdio.h>


VOID Mystdout_PrintDigit( UINT val )
{
	UINT print_lvl;

	if( val == 0 ) {
		stdout->put( '0', NULL );
	}

	// 最上位桁までスキップ
	for( print_lvl = 10000; print_lvl != 0; print_lvl /= 10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	Mystdout_PrintDigitN( val, print_lvl );
}

