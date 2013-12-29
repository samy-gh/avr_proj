
#ifndef __REPORT_ID_H__
#define __REPORT_ID_H__

#include "libhid.h"

#define D_REPORT_ID_1	(1)
#define D_REPORT_ID_2	(2)
#define D_REPORT_ID_3	(3)

extern int ReportWrite( const T_HID_HDL* const libhid_handle, unsigned char* const buf, const int length );
extern int ReportRead( const T_HID_HDL* const libhid_handle, unsigned char* const buf, const int length, const int id );

#endif /* __REPORT_ID_H__ */

