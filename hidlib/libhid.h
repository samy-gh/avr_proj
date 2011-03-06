/* libhid.h */

#ifndef __LIBHID_H__
#define __LIBHID_H__

typedef void T_HID_HDL;

typedef enum {
	E_HID_ERR_OK = 0,
	E_HID_ERR_NG
} E_HID_ERR;

extern E_HID_ERR HidInit( void );
extern void HidFree( void );
extern const T_HID_HDL* HidOpen( const char* const my_manufacturer, const char* const my_product );
extern void HidClose( const T_HID_HDL* const handle );
extern int HidRead( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id );
extern int HidWrite( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id );


#endif /* __LIBHID_H__ */

