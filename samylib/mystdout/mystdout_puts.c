
// mystdout
#include "mystdout.h"

// samylib
#include "my_typedef.h"

// STD-C
#include <stdio.h>


VOID Mystdout_Puts( const CHAR* str )
{
	while( *str ) {
		stdout->put( *str++, NULL );
	}
}

