#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhid.h"
#include "report_id.h"

#define	MY_Manufacturer	"SAMYNET"
#define	MY_Product		"HIDtest"

int main( int argc, char* argv[] )
{
	const T_HID_HDL* hid_hdl;
	char buf[40];

	HidInit();

	hid_hdl = HidOpen( MY_Manufacturer, MY_Product );
	if( !hid_hdl ) {
		printf( "hidOpen error\n" );
		HidFree();
		exit( 1 );
	}
	memset( buf, '\xee', sizeof(buf) );
	buf[0] = 0;
	buf[1] = 1;
	buf[2] = 3;
	buf[3] = 5;
	buf[4] = 7;
	buf[5] = 11;
	buf[6] = 13;
	buf[7] = 17;
	ReportWrite( hid_hdl, buf, 7 );
	ReportWrite( hid_hdl, buf, 8 );
	
	HidClose( hid_hdl );

	HidFree();

	return 0;
}
