
#include "mystdout.h"
#include "my_typedef.h"

VOID Mystdout_PrintHex( UINT val )
{
	UINT print_lvl;

	// 最上位桁までスキップ
	for( print_lvl = 0x1000; print_lvl != 0; print_lvl /= 0x10 ) {
		if( (val / print_lvl) != 0 ) {
			break;
		}
	}

	Mystdout_PrintHexN( val, print_lvl );
}

