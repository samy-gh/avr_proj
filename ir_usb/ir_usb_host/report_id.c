#include <stdio.h>
#include "libhid.h"


typedef struct {
	unsigned char	report_id;
	unsigned char	bytes;
} T_REPORT_ID;


T_REPORT_ID g_ReportId[] = {
	{ 1,  7 },
	{ 2, 31 },
	{ 3, 39 }
};


int ReportWrite( const T_HID_HDL* const libhid_handle, unsigned char* const buf, const int length )
{
	unsigned int loopcnt;

	for( loopcnt = 0; loopcnt < (sizeof(g_ReportId) / sizeof(T_REPORT_ID)); loopcnt++ ) {
		if( g_ReportId[loopcnt].bytes >= length ) {
			return HidWrite( libhid_handle, (char*)buf, g_ReportId[loopcnt].bytes, g_ReportId[loopcnt].report_id );
		}
	}

	return 0;
}


int ReportRead( const T_HID_HDL* const libhid_handle, unsigned char* const buf, const int length, const int id )
{
	return HidRead( libhid_handle, (char*)buf, length, id );
}

