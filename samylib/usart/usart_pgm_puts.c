
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"


VOID Usart_PgmPuts( PGM_P p_str )
{
	UCHAR c;

	while( 1 ) {
		c = pgm_read_byte( p_str++ );
		if( c == '\0' ) {
			return;
		}
		Usart_Write( c );
	}
}

