#ifndef _CEC_H
#define _CEC_H

#include <stdint.h>
#include <stm32f4xx.h>

/*
 * CEC line GPIO port
 */ 
#define CEC_GPIO	GPIOA

/*
 * CEC line pin number
 */ 
#define CEC_PIN		0

/*
 * CEC Pus-up/Push-Down
 */
#define CEC_PUPD	1

/*
 * CEC line GPIO Clock Enable Register
 */
#define CEC_PIN_ENR		AHB1ENR

/*
 * CEC line GPIO Clocke Enable Register field
 */
#define CEC_PIN_ENR_GPIO	RCC_AHB1ENR_GPIOAEN

/*
 * CEC External Interrupt
 */
#define CEC_EXTI	EXTI0_IRQn

/*
 * CEC External Interrupt Configuration Register number
 */
#define CEC_EXTICR	0

/*
 * CEC External Interrupt Line
 */
#define CEC_EXTI_LINE	0

/*
 * CEC External Interrupt source input 
 * 0x1 = PB[x] pin
 * 0x3 = PD[x] pin
 */
#define CEC_EXTI_PIN	0x0

/*
 * CEC External Interrupt Mask Register
 */
#define CEC_EXTI_IMR	EXTI_IMR_MR0

/*
 * CEC External Interrupt Rising trigger selection register
 */
#define CEC_EXTI_RTSR	EXTI_RTSR_TR0

/*
 * CEC External Interrupt Falling trigger selection registre
 */
#define CEC_EXTI_FTSR	EXTI_FTSR_TR0

/*
 * CEC External Interrupt Pending Register
 */
#define CEC_EXTI_PR	EXTI_PR_PR0

/*
 * CEC External Interrupt Priority
 */
#define CEC_EXTI_PRIORITY	2

/*
 * Timings in us
 */ 
#define CEC_TIME_UNIT	100

/*
 * The start bit has to be validated by its low duration (a) and its total duration (b)
 *
 *                                  b
 *       |<------------------------------------------------->|
 *                        a
 *       |<-------------------------->|
 * ______                       .......______________________.......
 *       |                      :     |     :          :     |     :
 *       |                      :     |     :          :     |     :
 *       |______________________:_____|.....:          :.....|_____:____
 *       :                      :     :     :          :     :     :
 *       0ms                 3.5ms 3.7ms 3.9ms      4.3ms  4.5ms  4.7ms
 */
#define CEC_START_BIT_MIN_L	3500
#define CEC_START_BIT_NOM_L	3700
#define CEC_START_BIT_MAX_L	3900
#define CEC_START_BIT_NOM_H	8000
#define CEC_START_BIT_MIN_T	4300
#define CEC_START_BIT_NOM_T	4500
#define CEC_START_BIT_MAX_T	4700

#define CEC_DATAx_BIT_MIN_T	2050
#define CEC_DATAx_BIT_NOM_T	2400
#define CEC_DATAx_BIT_MAX_T	2750
#define CEC_DATAx_BIT_NOM_S	1050

#define CEC_DATA0_BIT_MIN_L	1300
#define CEC_DATA0_BIT_NOM_L	1500
#define CEC_DATA0_BIT_MAX_L	1700
#define CEC_DATA0_BIT_NOM_H	900

#define CEC_DATA1_BIT_MIN_L	400
#define CEC_DATA1_BIT_NOM_L	600
#define CEC_DATA1_BIT_MAX_L	800
#define CEC_DATA1_BIT_NOM_H	1800

#define CEC_DURATION(name)	((CEC_##name)/CEC_TIME_UNIT)

#define CEC_MESSAGE_MAX_LENGTH		16
#define CEC_MESSAGE_MAX_OPERAND_LENGTH	(CEC_MESSAGE_MAX_LENGTH-1)

#define CEC_ACK		1
#define CEC_NACK	0

#define CEC_LOGICAL_ADDRESS_TV				0x00
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1		0x01
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2		0x02
#define CEC_LOGICAL_ADDRESS_STB_1			0x03
#define CEC_LOGICAL_ADDRESS_PLAYBACK_1			0x04
#define CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM		0x05
#define CEC_LOGICAL_ADDRESS_STB_2			0x06
#define CEC_LOGICAL_ADDRESS_STB_3			0x07
#define CEC_LOGICAL_ADDRESS_PLAYBACK_2			0x08
#define CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3		0x09
#define CEC_LOGICAL_ADDRESS_FREE_USE			0x0e
#define CEC_LOGICAL_ADDRESS_UREGISTERED			0xf0
#define CEC_LOGICAL_ADDRESS_BROADCAST			0x0f

#define CEC_OPCODE_FEATURE_ABORT			0x00
#define CEC_OPCODE_IMAGE_VIEW_ON			0x04
#define CEC_OPCODE_TUNER_STEP_INCREMENT			0x05
#define CEC_OPCODE_TUNER_STEP_DECREMENT			0x06
#define CEC_OPCODE_TUNER_DEVICE_STATUS			0x07
#define CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS		0x08
#define CEC_OPCODE_RECORD_ON				0x09
#define CEC_OPCODE_RECORD_STATUS			0x0A
#define CEC_OPCODE_RECORD_OFF				0x0B
#define CEC_OPCODE_TEXT_VIEW_ON				0x0D
#define CEC_OPCODE_RECORD_TV_SCREEN			0x0F
#define CEC_OPCODE_GIVE_DECK_STATUS			0x1A
#define CEC_OPCODE_DECK_STATUS				0x1B
#define CEC_OPCODE_SET_MENU_LANGUAGE			0x32
#define CEC_OPCODE_CLEAR_ANALOG_TIMER			0x33
#define CEC_OPCODE_SET_ANALOG_TIMER			0x34
#define CEC_OPCODE_TIMER_STATUS				0x35
#define CEC_OPCODE_STANDBY				0x36
#define CEC_OPCODE_PLAY					0x41
#define CEC_OPCODE_DECK_CONTROL				0x42
#define CEC_OPCODE_TIMER_CLEARED_STATUS			0x43
#define CEC_OPCODE_USER_CONTROL_PRESSED			0x44
#define CEC_OPCODE_USER_CONTROL_RELEASED		0x45
#define CEC_OPCODE_GIVE_OSD_NAME			0x46
#define CEC_OPCODE_SET_OSD_NAME				0x47
#define CEC_OPCODE_SET_OSD_STRING			0x64
#define CEC_OPCODE_SET_TIMER_PROGRAM_TITLE		0x67
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST		0x70
#define CEC_OPCODE_GIVE_AUDIO_STATUS			0x71
#define CEC_OPCODE_SET_SYSTEM_AUDIO_MODE		0x72
#define CEC_OPCODE_REPORT_AUDIO_STATUS			0x7A
#define CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS	0x7D
#define CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS		0x7E
#define CEC_OPCODE_ROUTING_CHANGE			0x80
#define CEC_OPCODE_ROUTING_INFORMATION			0x81
#define CEC_OPCODE_ACTIVE_SOURCE			0x82
#define CEC_OPCODE_GIVE_PHYSICAL_ADDRESS		0x83
#define CEC_OPCODE_REPORT_PHYSICAL_ADDRESS		0x84
#define CEC_OPCODE_REQUEST_ACTIVE_SOURCE		0x85
#define CEC_OPCODE_SET_STREAM_PATH			0x86
#define CEC_OPCODE_DEVICE_VENDOR_ID			0x87
#define CEC_OPCODE_VENDOR_COMMAND			0x89
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN		0x8A
#define CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP		0x8B
#define CEC_OPCODE_GIVE_DEVICE_VENDOR_ID		0x8C
#define CEC_OPCODE_MENU_REQUEST				0x8D
#define CEC_OPCODE_MENU_STATUS				0x8E
#define CEC_OPCODE_GIVE_DEVICE_POWER_STATUS		0x8F
#define CEC_OPCODE_REPORT_POWER_STATUS			0x90
#define CEC_OPCODE_GET_MENU_LANGUAGE			0x91
#define CEC_OPCODE_SELECT_ANALOG_SERVICE		0x92
#define CEC_OPCODE_SELECT_DIGITAL_SERVICE		0x93
#define CEC_OPCODE_SET_DIGITAL_TIMER			0x97
#define CEC_OPCODE_CLEAR_DIGITAL_TIMER			0x99
#define CEC_OPCODE_SET_AUDIO_RATE			0x9A
#define CEC_OPCODE_INACTIVE_SOURCE			0x9D
#define CEC_OPCODE_CEC_VERSION				0x9E
#define CEC_OPCODE_GET_CEC_VERSION			0x9F
#define CEC_OPCODE_VENDOR_COMMAND_WITH_ID		0xA0
#define CEC_OPCODE_CLEAR_EXTERNAL_TIMER			0xA1
#define CEC_OPCODE_SET_EXTERNAL_TIMER			0xA2
#define CEC_OPCODE_REPORT_SHORT_AUDIO_DESCRIPTOR	0xA3
#define CEC_OPCODE_REQUEST_SHORT_AUDIO_DESCRIPTOR	0xA4
#define CEC_OPCODE_INIT_ARC				0xC0
#define CEC_OPCODE_REPORT_ARC_INITED			0xC1
#define CEC_OPCODE_REPORT_ARC_TERMINATED		0xC2
#define CEC_OPCODE_REQUEST_ARC_INIT			0xC3
#define CEC_OPCODE_REQUEST_ARC_TERMINATION		0xC4
#define CEC_OPCODE_TERMINATE_ARC			0xC5
#define CEC_OPCODE_CDC					0xF8
#define CEC_OPCODE_ABORT				0xFF
   
#define CEC_USER_CONTROL_Select				0x00
#define CEC_USER_CONTROL_Up				0x01
#define CEC_USER_CONTROL_Down				0x02
#define CEC_USER_CONTROL_Left				0x03
#define CEC_USER_CONTROL_Right				0x04
#define CEC_USER_CONTROL_RightUp			0x05
#define CEC_USER_CONTROL_RightDown			0x06
#define CEC_USER_CONTROL_LeftUp				0x07
#define CEC_USER_CONTROL_LeftDown			0x08
#define CEC_USER_CONTROL_RootMenu			0x09
#define CEC_USER_CONTROL_SetupMenu			0x0A
#define CEC_USER_CONTROL_ContentsMenu			0x0B
#define CEC_USER_CONTROL_FavoriteMenu			0x0C
#define CEC_USER_CONTROL_Exit				0x0D
#define CEC_USER_CONTROL_Number0			0x20
#define CEC_USER_CONTROL_Number1			0x21
#define CEC_USER_CONTROL_Number2			0x22
#define CEC_USER_CONTROL_Number3			0x23
#define CEC_USER_CONTROL_Number4			0x24
#define CEC_USER_CONTROL_Number5			0x25
#define CEC_USER_CONTROL_Number6			0x26
#define CEC_USER_CONTROL_Number7			0x27
#define CEC_USER_CONTROL_Number8			0x28
#define CEC_USER_CONTROL_Number9			0x29
#define CEC_USER_CONTROL_Dot				0x2A
#define CEC_USER_CONTROL_Enter				0x2B
#define CEC_USER_CONTROL_Clear				0x2C
#define CEC_USER_CONTROL_ChannelUp			0x30
#define CEC_USER_CONTROL_ChannelDown			0x31
#define CEC_USER_CONTROL_PreviousChannel		0x32
#define CEC_USER_CONTROL_SoundSelect			0x33
#define CEC_USER_CONTROL_InputSelect			0x34
#define CEC_USER_CONTROL_DisplayInformation		0x35
#define CEC_USER_CONTROL_Help				0x36
#define CEC_USER_CONTROL_PageUp				0x37
#define CEC_USER_CONTROL_PageDown			0x38
#define CEC_USER_CONTROL_Power				0x40
#define CEC_USER_CONTROL_VolumeUp			0x41
#define CEC_USER_CONTROL_VolumeDown			0x42
#define CEC_USER_CONTROL_Mute				0x43
#define CEC_USER_CONTROL_Play				0x44
#define CEC_USER_CONTROL_Stop				0x45
#define CEC_USER_CONTROL_Pause				0x46
#define CEC_USER_CONTROL_Record				0x47
#define CEC_USER_CONTROL_Rewind				0x48
#define CEC_USER_CONTROL_FastForward			0x49
#define CEC_USER_CONTROL_Eject				0x4A
#define CEC_USER_CONTROL_Forward			0x4B
#define CEC_USER_CONTROL_Backward			0x4C
#define CEC_USER_CONTROL_Angle				0x50
#define CEC_USER_CONTROL_Subpicture			0x51
#define CEC_USER_CONTROL_VideoOnDemand			0x52
#define CEC_USER_CONTROL_EPG				0x53
#define CEC_USER_CONTROL_TimerProgramming		0x54
#define CEC_USER_CONTROL_InitialConfig			0x55
#define CEC_USER_CONTROL_PlayFunction			0x60
#define CEC_USER_CONTROL_PausePlayFunction		0x61
#define CEC_USER_CONTROL_RecordFunction			0x62
#define CEC_USER_CONTROL_PauseRecordFunction		0x63
#define CEC_USER_CONTROL_StopFunction			0x64
#define CEC_USER_CONTROL_MuteFunction			0x65
#define CEC_USER_CONTROL_RestoreVolumeFunction		0x66
#define CEC_USER_CONTROL_TuneFunction			0x67
#define CEC_USER_CONTROL_SelectDiskFunction		0x68
#define CEC_USER_CONTROL_SelectAVInputFunction		0x69
#define CEC_USER_CONTROL_SelectAudioInputFunction	0x6A
#define CEC_USER_CONTROL_F1Blue				0x71
#define CEC_USER_CONTROL_F2Red				0x72
#define CEC_USER_CONTROL_F3Green			0x73
#define CEC_USER_CONTROL_F4Yellow			0x74
#define CEC_USER_CONTROL_F5				0x75
#define CEC_USER_CONTROL_MAX				0x7F

#define CEC_DEVICE_TYPE_TV			0
#define CEC_DEVICE_TYPE_RECORDING_DEVICE	1
#define CEC_DEVICE_TYPE_RESERVED		2
#define CEC_DEVICE_TYPE_STB			3
#define CEC_DEVICE_TYPE_PLAYBACK		4
#define CEC_DEVICE_TYPE_AUDIO			5
#define CEC_DEVICE_TYPE_SWITCH			6
#define CEC_DEVICE_TYPE_VIDEO_PROCESSOR		7

#define CEC_ABORT_REASON_UNRECOGNIZED_OPCODE			0
#define CEC_ABORT_REASON_NOT_IN_CORRECT_MODE_TO_RESPOND		1
#define CEC_ABORT_REASON_CANNOT_PROVIDE_SOURCE			2
#define CEC_ABORT_REASON_INVALID_OPERAND			3
#define CEC_ABORT_REASON_REFUSED				4

#define CEC_MENU_REQUEST_TYPE_ACTIVATE		0
#define CEC_MENU_REQUEST_TYPE_DEACTIVATE	1
#define CEC_MENU_REQUEST_TYPE_QUERY		2

#define CEC_MENU_STATE_ACTIVATED		0
#define CEC_MENU_STATE_DEACTIVATED		1

#define CEC_POWER_STATUS_ON				0
#define CEC_POWER_STATUS_STANDBY			1
#define CEC_POWER_STATUS_IN_TRANSITION_STANDBY_TO_ON	2
#define CEC_POWER_STATUS_IN_TRANSITION_ON_TO_STANDBY	3

typedef uint8_t cec_logical_address_t;
typedef uint8_t cec_opcode_t;
typedef uint8_t cec_device_type_t;
typedef uint8_t cec_abort_reason_t;
typedef uint8_t cec_menu_request_type_t;
typedef uint8_t cec_menu_state_t;
typedef uint8_t cec_power_status_t;
typedef uint8_t cec_user_control_t;

struct cec_message_header
{
	uint8_t initiator : 4;
	uint8_t follower  : 4;
};

struct cec_physical_address
{
	uint8_t A : 4;
	uint8_t B : 4;
	uint8_t C : 4;
	uint8_t D : 4;
};

struct cec_message_active_source
{
	struct cec_physical_address physical_address;
};

struct cec_message_routing_information
{
	struct cec_physical_address physical_address;
};

struct cec_message_set_stream_path
{
	struct cec_physical_address physical_address;
};

struct cec_message_routing_change
{
	struct cec_physical_address original_address;
	struct cec_physical_address new_address;
};

struct cec_message_report_physical_address
{
	struct cec_physical_address physical_address;
	cec_device_type_t device_type;
};

struct cec_rx_message
{
	struct cec_message_header header;
	uint8_t length;
	uint8_t ack;
	union
	{
		struct
		{
			uint8_t opcode;
			union
			{
				struct cec_message_active_source active_source;
				struct cec_message_routing_change routing_change;
				struct cec_message_routing_information routing_information;
				struct cec_message_set_stream_path set_stream_path;
				struct cec_message_report_physical_address report_physical_address;
				cec_power_status_t power_status;
				cec_menu_request_type_t menu_request_type;
				cec_menu_state_t menu_state;
				cec_user_control_t user_control_pressed;
				uint8_t buff[CEC_MESSAGE_MAX_OPERAND_LENGTH];
			} operand;
		} data;
		uint8_t buff[CEC_MESSAGE_MAX_LENGTH];
	} message;
};

#define CEC_RX_FILTER_ALL_DEVICES	0xffff
#define CEC_RX_FILTER_NO_DEVICES	0x0000

union logical_address
{
	uint16_t all;
	struct
	{
		uint16_t TV			: 1;
		uint16_t Recording_Device_1	: 1;
		uint16_t Recording_Device_2	: 1;
		uint16_t STB1			: 1;
		uint16_t DVD1			: 1;
		uint16_t Audio			: 1;
		uint16_t STB2			: 1;
		uint16_t STB3			: 1;
		uint16_t DVD2			: 1;
		uint16_t Recording_Device_3	: 1;
		uint16_t Reserved_10		: 1;
		uint16_t Reserved_11		: 1;
		uint16_t Reserved_12		: 1;
		uint16_t Reserved_13		: 1;
		uint16_t FreeUse		: 1;
		uint16_t Broadcast		: 1;
	} device;
};

struct cec_rx_filter
{
	union logical_address receive;
	struct
	{
		union logical_address follower;
		union logical_address initiator;
	} monitor;
};

typedef enum
{
	CEC_RESULT_OK = 0,
	CEC_ERROR_PARAM = 1,
	CEC_ERROR_RX_DROPPED = 2,
	CEC_ERROR_RX_START_BIT = 3,
	CEC_ERROR_RX_HEADER_BLOCK = 4,
	CEC_ERROR_RX_DATA_BLOCK = 5,
	CEC_ERROR_RX_MONITOR_NACK = 6,
	CEC_ERROR_RX_MONITOR_DATA_BLOCK = 7
} cec_result_t;

void cec_time_tick(void);
void cec_init(void);
cec_result_t cec_rx_message(struct cec_rx_message * message, struct cec_rx_filter * rx_filter);

const char * cec_logical_address_to_str(cec_logical_address_t addr);
const char * cec_opcode_to_str(cec_opcode_t opcode);
const char * cec_device_type_to_str(cec_device_type_t type);
const char * cec_abort_reason_to_str(cec_abort_reason_t reason);
const char * cec_menu_request_type_to_str(cec_menu_request_type_t req);
const char * cec_menu_state_to_str(cec_menu_state_t state);
const char * cec_power_status_to_str(cec_power_status_t status);
const char * cec_user_control_to_str(cec_user_control_t uc);
#endif //_CEC_H
