
#include "mystdout.h"
#include "my_typedef.h"
#include <stdio.h>

VOID Mystdout_Putc( const CHAR c )
{
	stdout->put( c, NULL );
}

