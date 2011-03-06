
// mystdout
#include "mystdout.h"

// samylib
#include "my_typedef.h"

// STD-C
#include <stdio.h>


VOID Mystdout_PrintHexN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	if( print_lvl == 0 ) {
		return;
	}

	for( ; print_lvl != 0; print_lvl /= 0x10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

		if( digit_1 >= 0xa ) {
			stdout->put( 'A' + digit_1 - 0xa, NULL );
		}
		else {
			stdout->put( '0' + digit_1, NULL );
		}
	}
}

