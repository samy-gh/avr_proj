/* libhid.c
 * original: HIDaspx, binzume.net
 */

#define DEBUG			1		// for DEBUG

#define DEBUG_PKTDUMP	0		// HID Reportパケットをダンプする.
#define DUMP_PRODUCT	1		// 製造者,製品名を表示.


#ifndef WIN32
#ifndef LIBUSB
#define LIBUSB	// USE libusb  http://libusb.sourceforge.net/
#endif
#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libhid.h"


#ifdef LIBUSB
#	include <usb.h>
#else
#	include <windows.h>
#	include "usbhid.h"
#	ifndef __GNUC__
#		pragma comment(lib, "setupapi.lib")
#	endif
#endif


//	obdev
#define MY_VID			0x16c0		/* 5824 in dec, stands for VOTI */
#define MY_PID_libusb	0x05dc		/* libusb:0x05dc, obdev's free PID */
#define MY_PID			0x05df		/* HID: 0x05df, obdev's free PID */



#if	DEBUG_PKTDUMP
static	void memdump(char *msg, char *buf, int len);
#endif



//----------------------------------------------------------------------------



#ifdef LIBUSB ///////////////////////////////////////////////////////////////


typedef struct {
	usb_dev_handle* handle;
} T_HID_HDL_LOCAL;

typedef struct usbDevice usbDevice_t;
#define USBRQ_HID_GET_REPORT	0x01
#define USBRQ_HID_SET_REPORT	0x09
#define USB_HID_REPORT_TYPE_FEATURE 3

struct usb_device	*gdev;				// @@@ add by tanioka


static int usbhidGetStringAscii( usb_dev_handle *dev, int index, char *buf, int buflen )
{
	char	buffer[256];
	int		rval, i;

	if( (rval = usb_get_string_simple( dev, index, buf, buflen )) >= 0 ) { /* use libusb version if it works */
		return rval;
	}
	rval = usb_control_msg( dev, USB_ENDPOINT_IN, USB_REQ_GET_DESCRIPTOR,
		(USB_DT_STRING << 8) + index, 0x0409, buffer, sizeof(buffer), 5000 );

	if( rval < 0 ) {
		return rval;
	}
	if( buffer[1] != USB_DT_STRING ) {
		*buf = 0;
		return 0;
	}
	if( (unsigned char)buffer[0] < rval ) {
		rval = (unsigned char)buffer[0];
	}
	rval /= 2;
	/* lossy conversion to ISO Latin1: */
	for( i=1; i<rval; i++ ) {
		if( i > buflen ) {				/* destination buffer overflow */
			break;
		}
		buf[i -1] = buffer[2 * i];
		if( buffer[2 * i +1] != 0 ) {	/* outside of ISO Latin1 range */
			buf[i -1] = '?';
		}
	}
	buf[i -1] = 0;
	return i -1;
}

E_HID_ERR HidInit( void )
{
	usb_init();
	return E_HID_ERR_OK;
}

void HidFree( void )
{
}

const T_HID_HDL* HidOpen( const char* const my_manufacturer, const char* const my_product )
{
	struct usb_bus		*bus;
	struct usb_device	*dev;
	usb_dev_handle		*handle = NULL;
	T_HID_HDL_LOCAL*	libhid_handle = NULL;
	const int			my_product_id = MY_PID;

	usb_find_busses();
	usb_find_devices();

	for( bus = usb_get_busses(); bus; bus = bus->next ) {
		for( dev = bus->devices; dev; dev = dev->next ) {
			gdev = dev;		/* @@@ add by tanioka */
#if DEBUG
			printf("dev=%04x:%04x\n",dev->descriptor.idVendor,dev->descriptor.idProduct);
#endif
			if( (dev->descriptor.idVendor == MY_VID)
			 && (dev->descriptor.idProduct == my_product_id) ) {
				char	string[256];
				int		len;
				handle = usb_open( dev ); /* we need to open the device in order to query strings */
				if(!handle){
					fprintf( stderr, "Warning: cannot open USB device: %s\n", usb_strerror() );
					continue;
				}
				if( usb_set_configuration( handle, dev->config->bConfigurationValue ) < 0 ) {
					fprintf( stderr, "!!usb_set_configuration Error.\n" );
				}
#if DEBUG
				printf( "!!usb_set_configuration.\n" );
#endif

				if( usb_claim_interface( handle, dev->config->interface->altsetting->bInterfaceNumber ) < 0 ) {
					fprintf( stderr, "!!usb_claim_interface Error.\n" );
				}
#if DEBUG
				printf( "usb_claim_interface.\n" );
#endif

				/* now check whether the names match: */
				len = usbhidGetStringAscii( handle, dev->descriptor.iManufacturer, string, sizeof(string) );
				if( len < 0 ) {
					fprintf( stderr, "Warning: cannot query manufacturer for device: %s\n", usb_strerror() );
				} else {
#if DEBUG
					printf( "seen device from vendor [%s]\n", string );
#endif
					if( strcmp( string, my_manufacturer ) == 0 ) {
						len = usbhidGetStringAscii( handle, dev->descriptor.iProduct, string, sizeof(string) );
						if( len < 0 ) {
							fprintf( stderr, "Warning: cannot query product for device: %s\n", usb_strerror() );
						} else {
#if DEBUG
							fprintf( stderr, "seen product [%s]\n", string );
#endif
							if( strcmp( string, my_product ) == 0 ) {
								break;
							}
						}
					}
				}
				usb_close( handle );
				handle = NULL;
			}
		}
	}
	if(handle != NULL){
		libhid_handle = malloc( sizeof(T_HID_HDL_LOCAL) );
		if( libhid_handle ) {
			libhid_handle->handle = handle;
		}
		else {
//			usb_reset( (void*)handle );
			usb_close( (void*)handle );
		}
	}
#if LINUX
	setuid(getuid());	// @@@ by iruka
#endif
	return (T_HID_HDL*)libhid_handle;
}

void HidClose( const T_HID_HDL* const handle )
{
	T_HID_HDL_LOCAL* const libhid_handle = (T_HID_HDL_LOCAL* const)handle;

	if( libhid_handle ) {
		usb_release_interface( (void*)libhid_handle->handle, gdev->config->interface->altsetting->bInterfaceNumber );
//		usb_reset( (void*)libhid_handle->handle );
		usb_close( (void*)libhid_handle->handle );

		libhid_handle->handle = NULL;
		free( libhid_handle );
	}
#if DEBUG
	printf("usb_close.\n");
#endif
}

int HidRead( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id )
{
	int rc;
	usb_dev_handle *hdl;
	const T_HID_HDL_LOCAL* const libhid_handle_local = (const T_HID_HDL_LOCAL* const)libhid_handle;

	if( !libhid_handle_local ) {
		return 0;
	}

	hdl = libhid_handle_local->handle;

	buf[0] = id;
/*
int usb_control_msg(usb_dev_handle *dev, int requesttype, 
	int request, int value, int index, char *bytes, int size, int timeout);
 */
	rc = usb_control_msg( hdl, USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_IN,
		USBRQ_HID_GET_REPORT, USB_HID_REPORT_TYPE_FEATURE << 8 | id , 0, buf, length, 5000 );

	if( rc == length ) {
		return rc +1;
	} else {
		fprintf( stderr, "Error hidRead(): %s (rc=%d), %d\n", usb_strerror(), rc, length );
		return 0;
	}
}

int HidWrite( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id )
{
	int rc;
	usb_dev_handle *hdl;
	const T_HID_HDL_LOCAL* const libhid_handle_local = (const T_HID_HDL_LOCAL* const)libhid_handle;

	if( !libhid_handle_local ) {
		return 0;
	}

	hdl = libhid_handle_local->handle;


	buf[0] = id;

	rc = usb_control_msg( hdl, USB_TYPE_CLASS | USB_RECIP_INTERFACE | USB_ENDPOINT_OUT,
		USBRQ_HID_SET_REPORT, USB_HID_REPORT_TYPE_FEATURE << 8 | (id & 0xff), 0, buf, length, 5000);

	if( rc == length ) {
		return rc;
	} else {
		fprintf( stderr, "Error hidWrite(): %s (rc=%d), %d \n", usb_strerror(), rc, length );
		return 0;
	}
}



#else // For Windows //////////////////////////////////////////////////////////////

// HID API (from w2k DDK)
static _HidD_GetAttributes			HidD_GetAttributes;
static _HidD_GetHidGuid			HidD_GetHidGuid;
#if 0
static _HidD_GetPreparsedData		HidD_GetPreparsedData;
static _HidD_FreePreparsedData		HidD_FreePreparsedData;
static _HidP_GetCaps				HidP_GetCaps;
static _HidP_GetValueCaps			HidP_GetValueCaps;
#endif
static _HidD_GetFeature			HidD_GetFeature;
static _HidD_SetFeature			HidD_SetFeature;
static _HidD_GetManufacturerString	HidD_GetManufacturerString;
static _HidD_GetProductString		HidD_GetProductString;

static HINSTANCE hHID_DLL = NULL;		// hid.dll handle

typedef struct {
	HANDLE handle;
} T_HID_HDL_LOCAL;


/*
 *	unicode を ASCIIに変換.
 */
static char *uni_to_string(char *t, unsigned short *u)
{
	char *buf = t;
	int c;
	// short を char 配列に入れなおす.
	while (1) {
		c = *u++;
		if (c == 0)
			break;
		if (c & 0xff00)
			c = '?';
		*t++ = c;
	}

	*t = 0;
	return buf;
}

/*	Manufacturer & Product name check.
 *	名前チェック : 成功=1  失敗=0 読み取り不能=(-1)
 */
static int check_product_string( HANDLE handle, const char* const my_manufacturer, const char* const my_product )
{
	unsigned short unicode[512];
	char string1[256];
	char string2[256];

	if( !HidD_GetManufacturerString( handle, unicode, sizeof(unicode) ) ) {
		return -1;
	}
	uni_to_string( string1, unicode );

	if( !HidD_GetProductString( handle, unicode, sizeof(unicode) ) ) {
		return -1;
	}
	uni_to_string( string2, unicode );

#if	DUMP_PRODUCT
	fprintf( stderr, "iManufacturer:%s\n", string1 );
	fprintf( stderr, "iProduct:%s\n", string2 );
#endif

	if( strcmp( string1, my_manufacturer) != 0 ) {
		return 0;
	}

	if( strcmp( string2, my_product ) != 0 ) {
		return 0;
	}

	return 1;					//合致した.
}

E_HID_ERR HidInit( void )
{
	if( hHID_DLL ) {
		return E_HID_ERR_OK;
	}

	hHID_DLL = LoadLibrary( "hid.dll" );
	if( !hHID_DLL ) {
		fprintf(stderr, "Error at Load 'hid.dll'\n");
		return E_HID_ERR_NG;
	}
	HidD_GetAttributes =
		(_HidD_GetAttributes) GetProcAddress( hHID_DLL,
											 "HidD_GetAttributes" );
	if( !HidD_GetAttributes ) {
		fprintf( stderr, "Error at HidD_GetAttributes\n" );
		return E_HID_ERR_NG;
	}
	HidD_GetHidGuid =
		(_HidD_GetHidGuid) GetProcAddress( hHID_DLL, "HidD_GetHidGuid" );
	if( !HidD_GetHidGuid ) {
		fprintf( stderr, "Error at HidD_GetHidGuid\n" );
		return E_HID_ERR_NG;
	}
#if 0
	HidD_GetPreparsedData =
		(_HidD_GetPreparsedData) GetProcAddress( hHID_DLL,
												"HidD_GetPreparsedData" );
	HidD_FreePreparsedData =
		(_HidD_FreePreparsedData) GetProcAddress( hHID_DLL,
												 "HidD_FreePreparsedData" );
	HidP_GetCaps =
		(_HidP_GetCaps) GetProcAddress( hHID_DLL, "HidP_GetCaps" );
	HidP_GetValueCaps =
		(_HidP_GetValueCaps) GetProcAddress( hHID_DLL, "HidP_GetValueCaps" );
#endif

//
	HidD_GetFeature =
		(_HidD_GetFeature) GetProcAddress( hHID_DLL, "HidD_GetFeature" );
	HidD_SetFeature =
		(_HidD_SetFeature) GetProcAddress( hHID_DLL, "HidD_SetFeature" );
	HidD_GetManufacturerString = (_HidD_GetManufacturerString)
		GetProcAddress( hHID_DLL, "HidD_GetManufacturerString" );
	HidD_GetProductString = (_HidD_GetProductString)
		GetProcAddress( hHID_DLL, "HidD_GetProductString" );

#if	DEBUG
	printf( "_HidD_GetFeature= %x\n", (int)HidD_GetFeature );
	printf( "_HidD_SetFeature= %x\n", (int)HidD_SetFeature );
#endif
	return E_HID_ERR_OK;
}

void HidFree( void )
{
	if( hHID_DLL ) {
		FreeLibrary( hHID_DLL );
	}
	hHID_DLL = NULL;
}

const T_HID_HDL* HidOpen( const char* const my_manufacturer, const char* const my_product )
{
	int f = 0;
	int i = 0;
	ULONG Needed, l;
	GUID HidGuid;
	HDEVINFO DeviceInfoSet;
	HIDD_ATTRIBUTES DeviceAttributes;
	SP_DEVICE_INTERFACE_DATA DevData;
	PSP_INTERFACE_DEVICE_DETAIL_DATA DevDetail;
	//SP_DEVICE_INTERFACE_DETAIL_DATA *MyDeviceInterfaceDetailData;
	HANDLE hHID = NULL;						// USB-IO dev handle
	T_HID_HDL_LOCAL*	libhid_handle = NULL;
	const int my_product_id = MY_PID;

	if( !hHID_DLL ) {
		return NULL;
	}

	DeviceAttributes.Size = sizeof(HIDD_ATTRIBUTES);
	DevData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	HidD_GetHidGuid( &HidGuid );

	DeviceInfoSet =
		SetupDiGetClassDevs( &HidGuid, NULL, NULL,
							DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );

	while( SetupDiEnumDeviceInterfaces( DeviceInfoSet, 0, &HidGuid, i++, &DevData ) ) {
		SetupDiGetDeviceInterfaceDetail( DeviceInfoSet, &DevData, NULL, 0, &Needed, 0 );
		l = Needed;
		DevDetail =
			(SP_DEVICE_INTERFACE_DETAIL_DATA *) GlobalAlloc( GPTR, l + 4 );
		DevDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		SetupDiGetDeviceInterfaceDetail( DeviceInfoSet, &DevData, DevDetail,
										l, &Needed, 0 );

		hHID = CreateFile( DevDetail->DevicePath,
						   GENERIC_READ | GENERIC_WRITE,
						   FILE_SHARE_READ | FILE_SHARE_WRITE,
						   NULL, OPEN_EXISTING,
//						   FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING,
						   0, NULL );

		GlobalFree( DevDetail );

		if( hHID == INVALID_HANDLE_VALUE ) {	// Can't open a device
			continue;
		}
		HidD_GetAttributes( hHID, &DeviceAttributes );

		// HIDaspかどうか調べる.
		if( (DeviceAttributes.VendorID == MY_VID)
		 && (DeviceAttributes.ProductID == my_product_id)
		 && (check_product_string( hHID, my_manufacturer, my_product ) == 1)
		 ) {
			f = 1;				// 発見された.
			libhid_handle = malloc( sizeof(T_HID_HDL_LOCAL) );
			if( libhid_handle ) {
				libhid_handle->handle = hHID;
			}
			else {
				CloseHandle( hHID );
				hHID = NULL;
			}
			break;
		} else {
			// 違ったら閉じる
			CloseHandle( hHID );
			hHID = NULL;
		}
	}
	SetupDiDestroyDeviceInfoList( DeviceInfoSet );

	return (T_HID_HDL*)libhid_handle;
}

void HidClose( const T_HID_HDL* const handle )
{
	T_HID_HDL_LOCAL* const libhid_handle = (T_HID_HDL_LOCAL* const)handle;

	if( !hHID_DLL ) {
		return;
	}

	if( libhid_handle ) {
		if( libhid_handle->handle ) {
			CloseHandle( libhid_handle->handle );
		}
		libhid_handle->handle = NULL;

		free( libhid_handle );
	}
}

/*
 *	HIDデバイスから HID Report を取得する.
 *	受け取ったバッファは先頭の１バイトに必ずReportIDが入っている.
 *
 *	id と length の組はデバイス側で定義されたものでなければならない.
 *
 *	戻り値はHidD_GetFeatureの戻り値( 0 = 失敗 )
 *
 */
int HidRead( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id )
{
	int rc;
	HANDLE h;
	const T_HID_HDL_LOCAL* const libhid_handle_local = (const T_HID_HDL_LOCAL* const)libhid_handle;

	if( !hHID_DLL ) {
		return 0;
	}

	if( !libhid_handle_local ) {
		return 0;
	}

	h = libhid_handle_local->handle;

	buf[0] = id;
	rc = HidD_GetFeature( h, buf, length );
#if	DEBUG_PKTDUMP
	memdump( "RD", buf, length );
#endif
	return rc;
}

/*
 *	HIDデバイスに HID Report を送信するする.
 *	送信バッファの先頭の１バイトにReportID を入れる処理は
 *	この関数内で行うので、先頭１バイトを予約しておくこと.
 *
 *	id と length の組はデバイス側で定義されたものでなければならない.
 *
 *	戻り値はHidD_SetFeatureの戻り値( 0 = 失敗 )
 *
 */
int HidWrite( const T_HID_HDL* const libhid_handle, char* const buf, const int length, const int id )
{
	int rc;
	HANDLE h;
	const T_HID_HDL_LOCAL* const libhid_handle_local = (const T_HID_HDL_LOCAL* const)libhid_handle;

	if( !hHID_DLL ) {
		return 0;
	}

	if( !libhid_handle_local ) {
		return 0;
	}

	h = libhid_handle_local->handle;

	buf[0] = id;
	rc = HidD_SetFeature( h, buf, length );
#if	DEBUG_PKTDUMP
	memdump( "WR", buf, length );
#endif
	return rc;
}


#endif	// LIBUSB


