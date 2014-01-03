
#include <strings.h>

#include "ir_frame.h"
#include "usb_proto.h"


static void irframe_set_ext( T_USB_PROTO_IR_CODE* const ir_code )
{
	ir_code->data[2]  = 0x04;
	ir_code->data[3]  = 0xFB;
	ir_code->bytes = 10;
}


static void irframe_set_header( T_USB_PROTO_IR_CODE* const ir_code )
{
	memset( ir_code, '\0', sizeof(*ir_code) );

	ir_code->frame_type = D_USB_IR_FRAME_TYPE_TOSHIBA;
	ir_code->bytes = 9;
	ir_code->bit_pos = 1;
	ir_code->bit_pos = 1;
	ir_code->data[0]  = 0xF2;
	ir_code->data[1]  = 0x0D;
	ir_code->data[2]  = 0x03;
	ir_code->data[3]  = 0xFC;
	ir_code->data[4]  = 0x01;
	ir_code->data[7]  = 0x00;
}


static int irframe_set_drv_mode( T_USB_PROTO_IR_CODE* const ir_code, const E_DRIVE_MODE d_mode )
{
	switch( d_mode ) {
		case E_DRIVE_MODE_AUTO:
			ir_code->data[6] = (ir_code->data[6] & 0xF0) | 0x00;
			break;
		case E_DRIVE_MODE_COOLER:
			ir_code->data[6] = (ir_code->data[6] & 0xF0) | 0x01;
			break;
		case E_DRIVE_MODE_DEHUMI:
			irframe_set_ext( ir_code );
			ir_code->data[4] |= 0x08;
			ir_code->data[5] = (ir_code->data[5] & 0xF0) | 0x04;
			ir_code->data[6] = (ir_code->data[6] & 0xF0) | 0x02;
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x10;	// ‚Ü‚¸‚Í40%Ý’è
			break;
		case E_DRIVE_MODE_HEATER:
			ir_code->data[6] = (ir_code->data[6] & 0xF0) | 0x03;
			break;
		default:
			return -1;
	}

	return 0;
}


static int irframe_set_templature( T_USB_PROTO_IR_CODE* const ir_code, const float templature )
{
	unsigned char temp_bincode;

	if( (templature < 17.0) || (templature > 30.0 ) ) {
		return -1;
	}

	temp_bincode = 0x2 * (templature - 17.0);

	ir_code->data[5] = (ir_code->data[5] & 0x07) | (temp_bincode << 3);

	return 0;
}


static int irframe_set_humi( T_USB_PROTO_IR_CODE* const ir_code, const E_HUMI humi )
{
	irframe_set_ext( ir_code );
	ir_code->data[4] |= 0x08;

	switch( humi ) {
		case E_HUMI_40:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x10;
			break;
		case E_HUMI_45:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x20;
			break;
		case E_HUMI_50:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x30;
			break;
		case E_HUMI_55:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x40;
			break;
		case E_HUMI_60:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x50;
			break;
		case E_HUMI_65:
			ir_code->data[8] = (ir_code->data[8] & 0x0F) | 0x60;
			break;
		default:
			return -1;
	}

	return 0;
}


static int irframe_set_strength( T_USB_PROTO_IR_CODE* const ir_code, const E_STRENGTH str )
{
	switch( str ) {
		case E_STRENGTH_NORMAL:
			break;
		case E_STRENGTH_POWERFUL:
			irframe_set_ext( ir_code );
			ir_code->data[4] |= 0x08;
			ir_code->data[8] = (ir_code->data[8] & 0xF0) | 0x01;
			break;
		case E_STRENGTH_SAVING:
			irframe_set_ext( ir_code );
			ir_code->data[4] |= 0x08;
			ir_code->data[8] = (ir_code->data[8] & 0xF0) | 0x04;
			break;
		default:
			return -1;
	}

	return 0;
}


static int irframe_set_fanspeed( T_USB_PROTO_IR_CODE* const ir_code, const E_FANSPEED fanspeed )
{
	switch( fanspeed ) {
		case E_FANSPEED_AUTO:
			ir_code->data[6] = (ir_code->data[6] & 0x0F) | 0x00;
			break;
		case E_FANSPEED_SILENT:
			ir_code->data[6] = (ir_code->data[6] & 0x0F) | 0x20;
			break;
		case E_FANSPEED_SLOW:
			ir_code->data[6] = (ir_code->data[6] & 0x0F) | 0x40;
			break;
		case E_FANSPEED_MID:
			ir_code->data[6] = (ir_code->data[6] & 0x0F) | 0x80;
			break;
		case E_FANSPEED_FAST:
			ir_code->data[6] = (ir_code->data[6] & 0x0F) | 0xC0;
			break;
		default:
			return -1;
	}

	return 0;
}


static int irframe_calc_sum( T_USB_PROTO_IR_CODE* const ir_code )
{
	unsigned int loopcnt;
	unsigned char xor_sum;

	xor_sum = ir_code->data[4];
	for( loopcnt = 5; loopcnt < (ir_code->bytes -1); loopcnt++ ) {
		xor_sum ^= ir_code->data[loopcnt];
	}

	ir_code->data[ir_code->bytes -1] = xor_sum;

	return 0;
}


int IrFrame_AirConStart( T_USB_PROTO_IR_CODE* const ir_code, const T_AIRCON_PARAM* const aircon_param )
{
	irframe_set_header( ir_code );

	if( irframe_set_drv_mode( ir_code, aircon_param->d_mode ) != 0 ) {
		return -1;
	}
	switch( aircon_param->d_mode ) {
		case E_DRIVE_MODE_AUTO:
		case E_DRIVE_MODE_COOLER:
		case E_DRIVE_MODE_HEATER:
			if( irframe_set_templature( ir_code, aircon_param->templature ) != 0 ) {
				return -1;
			}
			break;
		case E_DRIVE_MODE_DEHUMI:
			if( irframe_set_templature( ir_code, aircon_param->templature ) != 0 ) {
				return -1;
			}
			if( irframe_set_humi( ir_code, aircon_param->humi ) != 0 ) {
				return -1;
			}
			break;
		default:
			return -1;
	}

	if( irframe_set_strength( ir_code, aircon_param->str ) != 0 ) {
		return -1;
	}
	if( irframe_set_fanspeed( ir_code, aircon_param->fanspeed ) != 0 ) {
		return -1;
	}
	if( irframe_calc_sum( ir_code ) != 0 ) {
		return -1;
	}

	return 0;
}


int IrFrame_AirConStop( T_USB_PROTO_IR_CODE* const ir_code )
{
	ir_code->frame_type = D_USB_IR_FRAME_TYPE_TOSHIBA;
	ir_code->bytes = 13;
	ir_code->bit_pos = 1;
	ir_code->data[0]  = 0xF2;
	ir_code->data[1]  = 0x0D;
	ir_code->data[2]  = 0x07;
	ir_code->data[3]  = 0xF8;
	ir_code->data[4]  = 0x01;
	ir_code->data[5]  = 0x10;
	ir_code->data[6]  = 0x87;
	ir_code->data[7]  = 0x00;
	ir_code->data[8]  = 0x55;
	ir_code->data[9]  = 0x3C;
	ir_code->data[10] = 0x01;
	ir_code->data[11] = 0x02;
	ir_code->data[12] = 0xFC;

	return 0;
}

