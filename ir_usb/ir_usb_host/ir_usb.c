#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhid.h"
#include "report_id.h"
#include "usb_proto.h"

#define	MY_Manufacturer	"SAMYNET"
#define	MY_Product		"IR-USB"

void dump_buf( unsigned char* buf, unsigned int len )
{
	unsigned int loopcnt;

	for( loopcnt = 0; loopcnt < len; loopcnt++ ) {
		printf( " %02x", buf[loopcnt] );
	}
	printf( "\n" );
}


int test( const T_HID_HDL* const hid_hdl )
{
	unsigned char buf[40];
	int  result;

	memset( buf, '\xee', sizeof(buf) );
//	buf[0] = 0;		// ReportID
	buf[1] = 1;		// Command
	buf[2] = 3;
	buf[3] = 5;
	buf[4] = 7;
	buf[5] = 11;
	buf[6] = 13;
	buf[7] = 17;
	ReportWrite( hid_hdl, buf, 7 );
	buf[1] = 2;		// Command
	ReportWrite( hid_hdl, buf, 8 );

	memset( buf, '\0', sizeof(buf) );
	result = ReportRead( hid_hdl, buf, 7, 1 );
	printf( "ReportRead result = %d\n", result );
	if( result == 1 ) {
		dump_buf( buf, 7 );
	}
	memset( buf, '\0', sizeof(buf) );
	result = ReportRead( hid_hdl, buf, 39, 3 );
	printf( "ReportRead result = %d\n", result );
	if( result == 1 ) {
		dump_buf( buf, 31 );
	}

	return 0;
}


int send_ir_eeprom( const T_HID_HDL* const hid_hdl )
{
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_WRITE)];
			T_USB_PROTO_WRITE	msg;
		}buf;
	} w_msg;
	unsigned char w_len;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_READ)];
			T_USB_PROTO_READ	msg;
		}buf;
	} r_msg;

	int  result;

	// コマンド送信
	memset( &w_msg, '\xee', sizeof(w_msg) );

	w_msg.buf.msg.command = D_CMD_IR_W_SEND_EEPROM;
	w_msg.buf.msg.len = 0;
	w_len = 3;			// report_id + command + len

	ReportWrite( hid_hdl, &w_msg.head, w_len );	// 送信時のreport_idは自動選択


	// 応答受信

	memset( &r_msg, '\0', sizeof(r_msg) );
	result = ReportRead( hid_hdl, &r_msg.head, 7, D_REPORT_ID_1 );
	printf( "ReportRead result = %d\n", result );
	if( result != 1 ) {
		return -1;
	}

//	dump_buf( &r_msg.head, sizeof(r_msg) );

	printf( "command: %02X\n", r_msg.buf.msg.command );
	printf( "retcd: %u\n", r_msg.buf.msg.retcd );
	printf( "len: %u\n", r_msg.buf.msg.len );

	return 0;
}


int send_ir_host( const T_HID_HDL* const hid_hdl )
{
	T_USB_PROTO_IR_CODE*	ir_code;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_WRITE)];
			T_USB_PROTO_WRITE	msg;
		}buf;
	} w_msg;
	unsigned char w_len;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_READ)];
			T_USB_PROTO_READ	msg;
		}buf;
	} r_msg;

	int  result;

	// コマンド送信
	memset( &w_msg, '\xee', sizeof(w_msg) );

	w_msg.buf.msg.command = D_CMD_IR_W_SEND_HOST;
	w_msg.buf.msg.len = 0;
	w_len = 3;			// report_id + command + len

	ir_code = (T_USB_PROTO_IR_CODE*)w_msg.buf.msg.data;
	ir_code->frame_type = D_USB_IR_FRAME_TYPE_SONY;
	ir_code->bytes = 2;
	ir_code->bit_pos = 2;
	ir_code->data[0] = 0xA9;
	ir_code->data[1] = 0x14;

	w_len += sizeof(T_USB_PROTO_IR_CODE);		// data

	ReportWrite( hid_hdl, &w_msg.head, w_len );	// 送信時のreport_idは自動選択


	// 応答受信

	memset( &r_msg, '\0', sizeof(r_msg) );
	result = ReportRead( hid_hdl, &r_msg.head, 7, D_REPORT_ID_1 );
	printf( "ReportRead result = %d\n", result );
	if( result != 1 ) {
		return -1;
	}

//	dump_buf( &r_msg.head, sizeof(r_msg) );

	printf( "command: %02X\n", r_msg.buf.msg.command );
	printf( "retcd: %u\n", r_msg.buf.msg.retcd );
	printf( "len: %u\n", r_msg.buf.msg.len );

	return 0;
}


int get_ir_eeprom( const T_HID_HDL* const hid_hdl )
{
	unsigned int				loopcnt;
	const T_USB_PROTO_IR_CODE*	ir_code;
	int							result;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_WRITE)];
			T_USB_PROTO_WRITE	msg;
		}buf;
	} w_msg;
	unsigned char w_len;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_READ)];
			T_USB_PROTO_READ	msg;
		}buf;
	} r_msg;

	// コマンド送信
	memset( &w_msg, '\xee', sizeof(w_msg) );

	w_msg.buf.msg.command = D_CMD_IR_W_GET_EEPROM;
	w_msg.buf.msg.len = 0;
	w_len = 3;			// report_id + command + len

	ReportWrite( hid_hdl, &w_msg.head, w_len );	// 送信時のreport_idは自動選択


	// 応答受信

	memset( &r_msg, '\0', sizeof(r_msg) );
	result = ReportRead( hid_hdl, &r_msg.head, 39, D_REPORT_ID_3 );
	printf( "ReportRead result = %d\n", result );
	if( result == 0 ) {
		return -1;
	}

//	dump_buf( &r_msg.head, sizeof(r_msg) );

	printf( "command: %02X\n", r_msg.buf.msg.command );
	printf( "retcd: %u\n", r_msg.buf.msg.retcd );
	printf( "len: %u\n", r_msg.buf.msg.len );

	ir_code = (const T_USB_PROTO_IR_CODE*)r_msg.buf.msg.data;

	printf( "frame_type: %u\n", ir_code->frame_type );
	printf( "bytes: %u\n", ir_code->bytes );
	printf( "bit_pos: %u\n", ir_code->bit_pos );
	printf( "data:" );
	for( loopcnt = 0; loopcnt < ir_code->bytes; loopcnt++ ) {
		printf( " %02X", ir_code->data[loopcnt] );
	}
	printf( "\n" );

	return 0;
}


int set_ir_eeprom( const T_HID_HDL* const hid_hdl )
{
	T_USB_PROTO_IR_CODE*	ir_code;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_WRITE)];
			T_USB_PROTO_WRITE	msg;
		}buf;
	} w_msg;
	unsigned char w_len;
	struct {
		unsigned char			head;	// ReportIDになる
		union {
			unsigned char		byte[sizeof(T_USB_PROTO_READ)];
			T_USB_PROTO_READ	msg;
		}buf;
	} r_msg;

	int  result;

	// コマンド送信
	memset( &w_msg, '\xee', sizeof(w_msg) );

	w_msg.buf.msg.command = D_CMD_IR_R_SET_EEPROM;
	w_msg.buf.msg.len = 0;
	w_len = 3;			// report_id + command + len

	ir_code = (T_USB_PROTO_IR_CODE*)w_msg.buf.msg.data;
	ir_code->frame_type = D_USB_IR_FRAME_TYPE_SONY;
	ir_code->bytes = 2;
	ir_code->bit_pos = 2;
	ir_code->data[0] = 0xA9;
	ir_code->data[1] = 0x14;

	w_len += sizeof(T_USB_PROTO_IR_CODE);		// data

	ReportWrite( hid_hdl, &w_msg.head, w_len );	// 送信時のreport_idは自動選択


	// 応答受信

	memset( &r_msg, '\0', sizeof(r_msg) );
	result = ReportRead( hid_hdl, &r_msg.head, 7, D_REPORT_ID_1 );
	printf( "ReportRead result = %d\n", result );
	if( result != 1 ) {
		return -1;
	}

//	dump_buf( &r_msg.head, sizeof(r_msg) );

	printf( "command: %02X\n", r_msg.buf.msg.command );
	printf( "retcd: %u\n", r_msg.buf.msg.retcd );
	printf( "len: %u\n", r_msg.buf.msg.len );

	return 0;
}


void print_usage( void )
{
	printf( "Usage: ir_usb <cmd_no>\n" );
}


int main( int argc, char* argv[] )
{
	const T_HID_HDL* hid_hdl;

	if( argc < 2 ) {
		print_usage();
		return 64;
	}

	HidInit();

	hid_hdl = HidOpen( MY_Manufacturer, MY_Product );
	if( !hid_hdl ) {
		printf( "hidOpen error\n" );
		HidFree();
		exit( 1 );
	}

	switch( atoi(argv[1]) ) {
		case 0:
			test( hid_hdl );
			break;
		case 1:
			send_ir_eeprom( hid_hdl );
			break;
		case 2:
			send_ir_host( hid_hdl );
			break;
		case 3:
			get_ir_eeprom( hid_hdl );
			break;
		case 4:
			set_ir_eeprom( hid_hdl );
			break;
		default:
			print_usage();
			break;
	}


	HidClose( hid_hdl );

	HidFree();

	return 0;
}
