
#include "mystdout.h"
#include "my_typedef.h"
#include <stdio.h>

VOID Mystdout_Puts( const CHAR* str )
{
	while( *str ) {
		stdout->put( *str++, NULL );
	}
}

