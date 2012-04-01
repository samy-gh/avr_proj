
// softtimer
#include "framework_msec2clock.h"

// samylib
#include "my_typedef.h"


static ULONG gul_clock_count = 0;


/*----------------------------------------------*/
/* msecをクロックカウント数に変換				*/
/*----------------------------------------------*/

VOID msec2clock_init( ULONG clock_count )
{
	gul_clock_count = clock_count;
}

ULONG msec2clock( ULONG ul_msec )
{
	ULONG ul_sec;
	ULONG ul_ret;

	if( ul_msec == 0 ) {
		return 0UL;
	}

	ul_sec = ul_msec / 1000;
	ul_msec %= 1000;

	ul_ret = ul_sec * gul_clock_count
		   + ul_msec * gul_clock_count / 1000;
	if( ul_ret == 0 ) {
		ul_ret = 1;
	}

	return ul_ret;
}

