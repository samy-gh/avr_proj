
// usart
#include "usart.h"

// samylib
#include "my_typedef.h"


VOID Usart_Puts( const CHAR* str )
{
	while( *str ) {
		Usart_Write( *str++ );
	}
}

