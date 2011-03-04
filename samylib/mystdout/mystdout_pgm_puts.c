
#include "mystdout.h"
#include "my_typedef.h"
#include <stdio.h>

VOID Mystdout_PgmPuts( PGM_P p_str )
{
	UCHAR c;

	while( 1 ) {
		c = pgm_read_byte( p_str++ );
		if( c == '\0' ) {
			return;
		}
		stdout->put( c, NULL );
	}
}

