
#ifndef __USB_PROTO_H__
#define __USB_PROTO_H__

//
// Writeメッセージ
//

// EEPROMに保存されているIRコードを送出する
// len = 0
#define D_CMD_IR_W_SEND_EEPROM		(0x01)

// HostからDeviceに送ったIRコードを送出する
// len = 可変
// data = 可変
#define D_CMD_IR_W_SEND_HOST		(0x02)

// EEPROMの内容をHostに読みだす
// len = 0
#define D_CMD_IR_W_GET_EEPROM		(0x03)

// HostからDeviceに送ったIRコードをEEPROMに保存する
// len = 可変
// data = 可変
#define D_CMD_IR_W_SET_EEPROM		(0x04)


typedef struct {
	unsigned char		command;	// Wコマンド
	unsigned char		len;		// データlength
	unsigned char		data[35];	// データ(optional)
} T_USB_PROTO_WRITE;


//
// Readメッセージ
//

// EEPROMに保存されているIRコードを送出する
// len = 0
#define D_CMD_IR_R_SEND_EEPROM		(0x81)

// HostからDeviceに送ったIRコードを送出する
// len = 0
#define D_CMD_IR_R_SEND_HOST		(0x82)

// EEPROMの内容をHostに読みだす
// len = 可変
// data = 可変
#define D_CMD_IR_R_GET_EEPROM		(0x83)

// HostからDeviceに送ったIRコードをEEPROMに保存する
// len = 0
#define D_CMD_IR_R_SET_EEPROM		(0x84)


#define D_RETCD_OK					(0)
#define D_RETCD_ERR					(1)

typedef struct {
	unsigned char		command;	// Rコマンド
	unsigned char		retcd;		// リターンコード
	unsigned char		len;		// データlength
	unsigned char		data[34];	// データ(optional)
} T_USB_PROTO_READ;



//
// IRコードコンテナ
//

// frame_type
#define D_USB_IR_FRAME_TYPE_UNKNOWN		(0)
#define D_USB_IR_FRAME_TYPE_NEC			(1)
#define D_USB_IR_FRAME_TYPE_AEHA		(2)
#define D_USB_IR_FRAME_TYPE_SONY		(3)
#define D_USB_IR_FRAME_TYPE_TOSHIBA		(4)

typedef struct {
	unsigned char	frame_type;	// フレーム種別
	unsigned char	bytes;		// データバイト長
	unsigned char	bit_pos;	// 最終ビット位置
	unsigned char	data[32];	// データ
} T_USB_PROTO_IR_CODE;


#endif //__USB_PROTO_H__

