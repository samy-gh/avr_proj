
#include "usart.h"
#include "my_typedef.h"

VOID Usart_PrintDigitN( UINT val, UINT print_lvl )
{
	UINT digit_1;

	for( ; print_lvl != 0; print_lvl /= 10 ) {
		digit_1 = val / print_lvl;
		val %= print_lvl;

//		if( digit_1 < 10 ) {
			Usart_Write( '0' + digit_1 );
//		}
//		else {
//			Usart_Write( '?' );
//		}
	}
}

