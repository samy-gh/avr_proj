
#ifndef __USB_PROTO_H__
#define __USB_PROTO_H__

//
// Write���b�Z�[�W
//

// EEPROM�ɕۑ�����Ă���IR�R�[�h�𑗏o����
// len = 0
#define D_CMD_IR_W_SEND_EEPROM		(0x01)

// Host����Device�ɑ�����IR�R�[�h�𑗏o����
// len = ��
// data = ��
#define D_CMD_IR_W_SEND_HOST		(0x02)

// EEPROM�̓��e��Host�ɓǂ݂���
// len = 0
#define D_CMD_IR_W_GET_EEPROM		(0x03)

// Host����Device�ɑ�����IR�R�[�h��EEPROM�ɕۑ�����
// len = ��
// data = ��
#define D_CMD_IR_W_SET_EEPROM		(0x04)


typedef struct {
	unsigned char		command;	// W�R�}���h
	unsigned char		len;		// �f�[�^length
	unsigned char		data[35];	// �f�[�^(optional)
} T_USB_PROTO_WRITE;


//
// Read���b�Z�[�W
//

// EEPROM�ɕۑ�����Ă���IR�R�[�h�𑗏o����
// len = 0
#define D_CMD_IR_R_SEND_EEPROM		(0x81)

// Host����Device�ɑ�����IR�R�[�h�𑗏o����
// len = 0
#define D_CMD_IR_R_SEND_HOST		(0x82)

// EEPROM�̓��e��Host�ɓǂ݂���
// len = ��
// data = ��
#define D_CMD_IR_R_GET_EEPROM		(0x83)

// Host����Device�ɑ�����IR�R�[�h��EEPROM�ɕۑ�����
// len = 0
#define D_CMD_IR_R_SET_EEPROM		(0x84)


#define D_RETCD_OK					(0)
#define D_RETCD_ERR					(1)

typedef struct {
	unsigned char		command;	// R�R�}���h
	unsigned char		retcd;		// ���^�[���R�[�h
	unsigned char		len;		// �f�[�^length
	unsigned char		data[34];	// �f�[�^(optional)
} T_USB_PROTO_READ;



//
// IR�R�[�h�R���e�i
//

// frame_type
#define D_USB_IR_FRAME_TYPE_UNKNOWN		(0)
#define D_USB_IR_FRAME_TYPE_NEC			(1)
#define D_USB_IR_FRAME_TYPE_AEHA		(2)
#define D_USB_IR_FRAME_TYPE_SONY		(3)
#define D_USB_IR_FRAME_TYPE_TOSHIBA		(4)

typedef struct {
	unsigned char	frame_type;	// �t���[�����
	unsigned char	bytes;		// �f�[�^�o�C�g��
	unsigned char	bit_pos;	// �ŏI�r�b�g�ʒu
	unsigned char	data[32];	// �f�[�^
} T_USB_PROTO_IR_CODE;


#endif //__USB_PROTO_H__

