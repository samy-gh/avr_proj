
// mystdout
#include "mystdout.h"

// samylib
#include "my_typedef.h"

// STD-C
#include <stdio.h>


VOID Mystdout_Putc( const CHAR c )
{
	stdout->put( c, NULL );
}

