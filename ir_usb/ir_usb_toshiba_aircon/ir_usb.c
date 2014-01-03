#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhid.h"
#include "report_id.h"
#include "usb_proto.h"
#include "ir_frame.h"

#define	MY_Manufacturer	"SAMYNET"
#define	MY_Product		"IR-USB"

int send_ir_host( const T_HID_HDL* const hid_hdl, const T_USB_PROTO_IR_CODE* const ir_code )
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

	w_msg.buf.msg.command = D_CMD_IR_W_SEND_HOST;
	w_msg.buf.msg.len = 0;
	w_len = 3;			// report_id + command + len

	memcpy( w_msg.buf.msg.data, ir_code, sizeof(*ir_code) );
	w_len += sizeof(T_USB_PROTO_IR_CODE);		// data

	ReportWrite( hid_hdl, &w_msg.head, w_len );	// 送信時のreport_idは自動選択



	// 応答受信
	memset( &r_msg, '\0', sizeof(r_msg) );
	result = ReportRead( hid_hdl, &r_msg.head, 7, D_REPORT_ID_1 );
	if( result != 1 ) {
		return -1;
	}


	printf( "retcd: %u\n", r_msg.buf.msg.retcd );

	return 0;
}


void print_usage( void )
{
	printf( "Usage: ir_aircon <auto|cooler|heater|dehumi> <temp 17.0~30.0> <norm|powerful|saving> <auto|silent|slow|mid|fast>\n" );
	printf( "Usage: ir_aircon <dehumi> <humi 40~65> <norm|powerful|saving>\n" );
	printf( "Usage: ir_aircon <stop>\n" );
}


int parse_arg_d_mode( T_AIRCON_PARAM* const aircon_param, const char* const argv )
{
	if( strcmp( argv, "auto" ) == 0 ) {
		aircon_param->d_mode = E_DRIVE_MODE_AUTO;
	}
	else
	if( strcmp( argv, "cooler" ) == 0 ) {
		aircon_param->d_mode = E_DRIVE_MODE_COOLER;
	}
	else
	if( strcmp( argv, "heater" ) == 0 ) {
		aircon_param->d_mode = E_DRIVE_MODE_HEATER;
	}
	else {
		return -1;
	}

	return 0;
}


int parse_arg_humi( T_AIRCON_PARAM* const aircon_param, const char* const argv )
{
	switch( atoi( argv ) ) {
		case 40:
			aircon_param->humi = E_HUMI_40;
			break;
		case 45:
			aircon_param->humi = E_HUMI_45;
			break;
		case 50:
			aircon_param->humi = E_HUMI_50;
			break;
		case 55:
			aircon_param->humi = E_HUMI_55;
			break;
		case 60:
			aircon_param->humi = E_HUMI_60;
			break;
		case 65:
			aircon_param->humi = E_HUMI_65;
			break;
		default:
			return -1;
	}

	return 0;
}


int parse_arg_strength( T_AIRCON_PARAM* const aircon_param, const char* const argv )
{
	if( strcmp( argv, "norm" ) == 0 ) {
		aircon_param->str = E_STRENGTH_NORMAL;
	}
	else
	if( strcmp( argv, "powerful" ) == 0 ) {
		aircon_param->str = E_STRENGTH_POWERFUL;
	}
	else
	if( strcmp( argv, "saving" ) == 0 ) {
		aircon_param->str = E_STRENGTH_SAVING;
	}
	else {
		return -1;
	}

	return 0;
}


int parse_arg_fanspeed( T_AIRCON_PARAM* const aircon_param, const char* const argv )
{
	if( strcmp( argv, "auto" ) == 0 ) {
		aircon_param->fanspeed = E_FANSPEED_AUTO;
	}
	else
	if( strcmp( argv, "silent" ) == 0 ) {
		aircon_param->fanspeed = E_FANSPEED_SILENT;
	}
	else
	if( strcmp( argv, "slow" ) == 0 ) {
		aircon_param->fanspeed = E_FANSPEED_SLOW;
	}
	else
	if( strcmp( argv, "mid" ) == 0 ) {
		aircon_param->fanspeed = E_FANSPEED_MID;
	}
	else
	if( strcmp( argv, "fast" ) == 0 ) {
		aircon_param->fanspeed = E_FANSPEED_FAST;
	}
	else {
		return -1;
	}

	return 0;
}


void parse_arg( T_AIRCON_PARAM* const aircon_param, int argc, char* argv[] )
{
	if( argc == 2 ) {
		if( strcmp( argv[1], "stop" ) == 0 ) {
			aircon_param->d_mode = E_DRIVE_MODE_STOP;
			return;
		}
	}
	else
	if( argc == 4 ) {
		while( 1 ) {
			if( strcmp( argv[1], "dehumi" ) != 0 ) {
				break;
			}
			aircon_param->d_mode = E_DRIVE_MODE_DEHUMI;
			aircon_param->templature = 24.0;
			aircon_param->fanspeed = E_FANSPEED_AUTO;

			if( parse_arg_humi( aircon_param, argv[2] ) != 0 ) {
				break;
			}

			if( parse_arg_strength( aircon_param, argv[3] ) != 0 ) {
				break;
			}

			return;
		}
	}
	else
	if( argc == 5 ) {
		while( 1 ) {
			if( parse_arg_d_mode( aircon_param, argv[1] ) != 0 ) {
				break;
			}

			aircon_param->templature = atof( argv[2] );


			if( parse_arg_strength( aircon_param, argv[3] ) != 0 ) {
				break;
			}

			if( parse_arg_fanspeed( aircon_param, argv[4] ) != 0 ) {
				break;
			}

			return;
		}
	}

	print_usage();
	exit( 64 );
}


int send_ir( const T_USB_PROTO_IR_CODE* const ir_code )
{
	const T_HID_HDL* hid_hdl;

	HidInit();

	hid_hdl = HidOpen( MY_Manufacturer, MY_Product );
	if( !hid_hdl ) {
		printf( "hidOpen error\n" );
		HidFree();
		exit( 2 );
	}

	send_ir_host( hid_hdl, ir_code );

	HidClose( hid_hdl );

	HidFree();

	return 0;
}


int main( int argc, char* argv[] )
{
	T_AIRCON_PARAM			aircon_param;
	T_USB_PROTO_IR_CODE		ir_code;

	parse_arg( &aircon_param, argc, argv );


	printf( "d_mode = %u\n", aircon_param.d_mode );
	printf( "temp = %.1f\n", aircon_param.templature );
	printf( "humi = %u\n", aircon_param.humi );
	printf( "strength = %u\n", aircon_param.str );
	printf( "fan = %u\n", aircon_param.fanspeed );


	if( aircon_param.d_mode != E_DRIVE_MODE_STOP ) {
		if( IrFrame_AirConStart( &ir_code, &aircon_param ) != 0 ) {
			printf( "parameter error.\n" );
			return 1;
		}
	}
	else {
		if( IrFrame_AirConStop( &ir_code ) != 0 ) {
			printf( "parameter error.\n" );
			return 1;
		}
	}

	send_ir( &ir_code );

	return 0;
}

