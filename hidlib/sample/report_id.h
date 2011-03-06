
#ifndef __REPORT_ID_H__
#define __REPORT_ID_H__

#include "libhid.h"


extern int ReportWrite( const T_HID_HDL* const libhid_handle, char* const buf, const int length );
extern int ReportRead( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id );

#endif /* __REPORT_ID_H__ */

