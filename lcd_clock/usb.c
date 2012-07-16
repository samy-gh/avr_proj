
// samylib
#include <my_typedef.h>
#include <mystdout.h>
#include <usart.h>

// usbdrv
#include <usbdrv/usbdrv.h>

// WinAVR
#include <util/delay.h>

// STD-C
#include <stdio.h>
#include <string.h>


/*

 「重要」
 usbHidReportDescriptor[] の内容を修正した時には、usbconfig.h の
 以下の行に、usbHidReportDescriptor[] の占めるサイズを正確に設定すること

 #define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    42

*/
PROGMEM char usbHidReportDescriptor[] = {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)

    0x85, 0x01,                    //   REPORT_ID (1)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x02,                    //   REPORT_ID (2)
    0x95, 0x1e,                    //   REPORT_COUNT (30)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0x85, 0x03,                    //   REPORT_ID (3)
    0x95, 0x26,                    //   REPORT_COUNT (38)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

    0xc0                           // END_COLLECTION
};


static uchar currentPosition;
static uchar bytesRemaining; // Receive Data Pointer

//
//	受信バッファ.
//
typedef struct {
	UCHAR id[1];
	UCHAR buf[39];
} ReportBuf_t;

static ReportBuf_t g_Report;

//	但し、先頭1バイトはReport IDを置く必要があるので,1バイトずれる.
#define	usbBody g_Report.buf
#define	buffer  g_Report.id


void dump_pkt( void )
{
	UCHAR* buf = buffer;
	UCHAR loopcnt = currentPosition;

	// 標準出力関数のテスト
	Usart_Set_Stdout();
	printf_P( PSTR("recv: len=%u :"), loopcnt );
	for( ; loopcnt != 0; loopcnt-- ) {
		printf( "%02X ", *buf );
		buf++;
	}
	putchar( '\n' );
}


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

// デフォルト以外の usb setup コマンドパケットをここで解釈する.
usbMsgLen_t usbFunctionSetup( uchar data[8] )
{
	usbRequest_t	*rq = (void *)data;

	if( (rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS ) {    /* class request type */
		return 0;
	}

	switch( rq->bRequest ) {
		case USBRQ_HID_GET_REPORT:
			g_Report.id[0] = rq->wValue.bytes[0];    /* store report ID */
			usbMsgPtr = g_Report.id;	/* 応答フェーズ用データを設定 */

#if 0
			Usart_Set_Stdout();
			printf_P( PSTR("recv: id=%u len=%u\n"), g_Report.id[0], rq->wLength.word );
#endif
			if( g_Report.id[0] == 1 ) {

			}
//			return 0;
			return rq->wLength.word;

		case USBRQ_HID_SET_REPORT:
			currentPosition = 0;                // initialize position index
			bytesRemaining = rq->wLength.word;  // store the amount of data requested
			if(	bytesRemaining > sizeof(g_Report)) { // limit to buffer size
				bytesRemaining = sizeof(g_Report);
			}
			return USB_NO_MSG;						// tell driver to use usbFunctionWrite()

		default:
			return 0;
	}
}

// コントロール転送が後続データを持っている場合、それを8バイト単位で受け取る.
//	・結果はg_Report構造体に入れる.
//	・全部受け取ったら、hidasp_main()を呼び出す.
uchar usbFunctionWrite( uchar *data, uchar len )
{
	UCHAR loopcnt;

	if( len > bytesRemaining ) {             // if this is the last incomplete chunk
		len = bytesRemaining;               // limit to the amount we can store
	}

	bytesRemaining -= len;
	for(loopcnt = 0; loopcnt < len; loopcnt++) {
		g_Report.id[currentPosition++] = *data++;
	}

	// 全部受け取ったら、バッファ内容を実行.
	if( bytesRemaining == 0 ) {
		dump_pkt();
		return 1;	// return 1 if we have all data
	}
	return 0;		// continue data.
}

/* ------------------------------------------------------------------------- */
// API
/* ------------------------------------------------------------------------- */

VOID Usb_Init( VOID )
{
	// USB初期化
	usbInit();
	usbDeviceDisconnect();
}

VOID Usb_Connect( const UINT delay_ms )
{
	/* Power ON delay (300ms) */
	_delay_ms( delay_ms );

	// USB接続開始
	usbDeviceConnect();
}

