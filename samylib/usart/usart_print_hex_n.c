
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"


VOID Usart_PrintHexN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	if( print_lvl == 0 ) {
		return;
	}

	for( ; print_lvl != 0; print_lvl /= 0x10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

		if( digit_1 >= 0xa ) {
			Usart_Write( 'A' + digit_1 - 0xa );
		}
		else {
			Usart_Write( '0' + digit_1 );
		}
	}
}

