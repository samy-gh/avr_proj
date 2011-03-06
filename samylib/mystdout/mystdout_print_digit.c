
// mystdout
#include "mystdout.h"

// samylib
#include "my_typedef.h"


VOID Mystdout_PrintDigit( UINT val )
{
	UINT print_lvl;

	// 最上位桁までスキップ
	for( print_lvl = 10000; print_lvl != 0; print_lvl /= 10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	Mystdout_PrintDigitN( val, print_lvl );
}

