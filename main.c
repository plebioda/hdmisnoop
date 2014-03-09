#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f4xx.h>
#include <cec.h>
#include <clock.h>
#include <usb.h>
#include <usb_cdc.h>
#include <usb_dev.h>
#include <usb_dbg.h>
#include <debug.h>

#define err(fmt, args...)	printf("error: " fmt, ## args)
#define USB_CDC_MAX_PACKET_SIZE0	64	
#define USB_CDC_VID			0x0483
#define USB_CDC_PID			0x5740
#define USB_CDC_STR_ID_LANG		0
#define USB_CDC_STR_ID_MANUFACTURER	1
#define USB_CDC_STR_ID_PRODUCT		2
#define USB_CDC_STR_ID_SERIAL		3
#define USB_CDC_NUM_CFG			1
#define USB_CDC_CMD_EP_NUMBER		2
#define USB_CDC_CMD_PACKET_SZE		8
#define USB_CDC_DATA_EP_NUMBER		1
#define USB_CDC_DATA_PACKET_SZE		0x40
#define USB_CDC_LANGID			0x409
#define USB_CDC_MANUFACTURER_STRING	"STMicroelectronics"
#define USB_CDC_PRODUCT_STRING		"STM32 Virtual ComPort in FS Mode"
#define USB_CDC_SERIAL_STRING		"00000000050C"

static usb_device_descriptor_t desc_device = 
{
	.bLength 		= sizeof(usb_device_descriptor_t),
	.bDescriptorType 	= USB_DESCRIPTOR_TYPE_DEVICE,
	.bcdUSB			= {0x00, 0x02},
	.bDeviceClass 		= 0x00,
	.bDeviceSubClass 	= 0x00,
	.bDeviceProtocol	= 0x00,
	.bMaxPacketSize0 	= USB_CDC_MAX_PACKET_SIZE0, 
	.idVendor		= USB_CDC_VID,
	.idProduct		= USB_CDC_PID,
	.bcdDevice		= {0x00, 0x02},
	.iManufacturer		= USB_CDC_STR_ID_MANUFACTURER,
	.iProduct		= USB_CDC_STR_ID_PRODUCT,
	.iSerialNumber		= USB_CDC_STR_ID_SERIAL,
	.bNumConfigurations	= USB_CDC_NUM_CFG
};

static struct 
{
	usb_configuration_descriptor_t 	configuration_descriptor;
	usb_interface_descriptor_t 	interface_descriptor;
	usb_cdc_header_descriptor_t 	header_functional_descriptor;
	usb_cdc_cm_descriptor_t 	cm_functional_descriptor;
	usb_cdc_acm_descriptor_t 	acm_functional_descriptor;
	usb_cdc_union_descriptor_t	union_functional_descriptor;
	uint8_t 			bSlaveInterface0;
	usb_endpoint_descriptor_t 	endpoint_2_descriptor;
	usb_interface_descriptor_t 	data_class_interface_descriptor;
	usb_endpoint_descriptor_t 	endpoint_out_descriptor;
	usb_endpoint_descriptor_t 	endpoint_in_descriptor;
} cdc_configuration_descriptor 		= {
	.configuration_descriptor	= {
		.bLength 		= sizeof(usb_configuration_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_CONFIGURATION,
		.wTotalLength		= sizeof(cdc_configuration_descriptor),
		.bNumInterfaces		= 2,
		.bConfigurationValue	= 1,
		.iConfiguration		= 0,
		.bmAttributes 		= {
			.RemoteWakeup	= 0,
			.SelfPowered	= 1,
			.reserved7	= 1
		},
		.bMaxPower 		= 0x32,
	},
	.interface_descriptor		= {
		.bLength		= sizeof(usb_interface_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber	= 0,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 1,
		.bInterfaceClass	= 2,
		.bInterfaceSubClass	= 2,
		.bInterfaceProtocol	= 1,
		.iInterface		= 0,
	},
	.header_functional_descriptor	= {
		.bFunctionLength	= sizeof(usb_cdc_header_descriptor_t),
		.bDescriptorType	= USB_CDC_DESCRIPTOR_TYPE_CS_INTERFACE,
		.bDescriptorSubtype	= USB_CDC_DESCRIPTOR_SUBTYPE_HEADER,
		.bcdCDC			= {0x01, 0x10},
	},
	.cm_functional_descriptor	= {
		.bFunctionLength	= sizeof(usb_cdc_cm_descriptor_t),
		.bDescriptorType	= USB_CDC_DESCRIPTOR_TYPE_CS_INTERFACE,
		.bDescriptorSubtype	= USB_CDC_DESCRIPTOR_SUBTYPE_CALL_MANAGEMENT,
		.bmCapabilities		= 0x00,
		.bDataInterface		= 1,
	},
	.acm_functional_descriptor	= {
		.bFunctionLength	= sizeof(usb_cdc_acm_descriptor_t),
		.bDescriptorType	= USB_CDC_DESCRIPTOR_TYPE_CS_INTERFACE,
		.bDescriptorSubtype	= USB_CDC_DESCRIPTOR_SUBTYPE_ABSTRACT_CONTROL_MANAGEMENT,
		.bmCapabilities		= 0x02,
	},
	.union_functional_descriptor	= {
		.bFunctionLength	= sizeof(usb_cdc_union_descriptor_t) + 1,
		.bDescriptorType	= USB_CDC_DESCRIPTOR_TYPE_CS_INTERFACE,
		.bDescriptorSubtype	= USB_CDC_DESCRIPTOR_SUBTYPE_UNION,
		.bMasterInterface	= 0,
	},
	.bSlaveInterface0		= 1,
	.endpoint_2_descriptor		= {
		.bLength		= sizeof(usb_endpoint_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress	= {
			.number		= USB_CDC_CMD_EP_NUMBER,
			.dir		= USB_ENDPOINT_DIR_IN,
			.reserved6_4	= 0,
		},
		.bmAttributes		= {
			.TransferType 	= USB_ENDPOINT_TYPE_INTERRUPT,
			.SyncType	= USB_ENDPOINT_SYNC_TYPE_NO,
			.UsageType	= USB_ENDPOINT_USAGE_TYPE_DATA,
			.reserved7_6	= 0,
		},
		.wMaxPacketSize		= USB_CDC_CMD_PACKET_SZE,
		.bInterval		= 0xFF,
	},
	.data_class_interface_descriptor= {
		.bLength		= sizeof(usb_interface_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_INTERFACE,
		.bInterfaceNumber	= 1,
		.bAlternateSetting	= 0,
		.bNumEndpoints		= 2,
		.bInterfaceClass	= 0x0A,
		.bInterfaceSubClass	= 0,
		.bInterfaceProtocol	= 0,
		.iInterface		= 0
	},
	.endpoint_out_descriptor	= {
		.bLength		= sizeof(usb_endpoint_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress 	= {
			.number		= USB_CDC_DATA_EP_NUMBER,
			.dir 		= USB_ENDPOINT_DIR_OUT,
			.reserved6_4	= 0
		},
		.bmAttributes		= {
			.TransferType 	= USB_ENDPOINT_TYPE_BULK,
			.SyncType	= USB_ENDPOINT_SYNC_TYPE_NO,
			.UsageType	= USB_ENDPOINT_USAGE_TYPE_DATA,
			.reserved7_6	= 0,
		},
		.wMaxPacketSize		= USB_CDC_DATA_PACKET_SZE,
		.bInterval		= 0

	},
	.endpoint_in_descriptor		= {
		.bLength		= sizeof(usb_endpoint_descriptor_t),
		.bDescriptorType	= USB_DESCRIPTOR_TYPE_ENDPOINT,
		.bEndpointAddress 	= {
			.number		= USB_CDC_DATA_EP_NUMBER,
			.dir 		= USB_ENDPOINT_DIR_IN,
			.reserved6_4	= 0
		},
		.bmAttributes		= {
			.TransferType 	= USB_ENDPOINT_TYPE_BULK,
			.SyncType	= USB_ENDPOINT_SYNC_TYPE_NO,
			.UsageType	= USB_ENDPOINT_USAGE_TYPE_DATA,
			.reserved7_6	= 0,
		},
		.wMaxPacketSize		= USB_CDC_DATA_PACKET_SZE,
		.bInterval		= 0
	}
};

typedef struct usb_langid_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wLANGID[1];
} PACKED usb_langid_descriptor_t;

uint8_t string_descriptor[USB_MAX_STRING_DESCRIPTOR];

usb_langid_descriptor_t langid_descriptor = {
	.bLength 		= sizeof(usb_langid_descriptor_t),
	.bDescriptorType  	= USB_DESCRIPTOR_TYPE_STRING,
	.wLANGID[0] 		= USB_CDC_LANGID 
};

#define CEC_BUFFER_SIZE		16
struct cec_message_buffer_item
{
	up_time_t time;
	cec_result_t result;
	struct cec_rx_message message;
};

struct 
{
	int size;
	int first;
	int last;
	struct cec_message_buffer_item items[CEC_BUFFER_SIZE];
} cec_message_buffer;

void print_message(cec_result_t result, up_time_t time, struct cec_rx_message * msg);

usb_ret_t usb_cdc_sof(struct usb_device * usbd)
{

}

usb_ret_t usb_cdc_get_device_descriptor(struct usb_device * usbd, usb_descriptor_type_t type, uint8_t index, struct usb_device_buffer * buff)
{
	if(NULL != buff)
	{
		switch(type)
		{
			case USB_DESCRIPTOR_TYPE_DEVICE:
				buff->ptr = (uint8_t*)&desc_device;
				buff->len = sizeof(desc_device);
				break;
			case USB_DESCRIPTOR_TYPE_CONFIGURATION:
				buff->ptr = (uint8_t*)&cdc_configuration_descriptor;
				buff->len = sizeof(cdc_configuration_descriptor);
				break;
			case USB_DESCRIPTOR_TYPE_STRING:
				dbg("STRING ID=%d\n", index);
				switch(index)
				{
					case USB_CDC_STR_ID_LANG:
						buff->ptr = (uint8_t*)&langid_descriptor;
						buff->len = sizeof(langid_descriptor);
						break;
					case USB_CDC_STR_ID_MANUFACTURER:
						buff->len = usb_dev_get_string_descriptor(USB_CDC_MANUFACTURER_STRING, string_descriptor, USB_MAX_STRING_DESCRIPTOR);
						buff->ptr = string_descriptor;
						break;
					case USB_CDC_STR_ID_PRODUCT:
						buff->len = usb_dev_get_string_descriptor(USB_CDC_PRODUCT_STRING, string_descriptor, USB_MAX_STRING_DESCRIPTOR);
						buff->ptr = string_descriptor;
						break;
					case USB_CDC_STR_ID_SERIAL:
						buff->len = usb_dev_get_string_descriptor(USB_CDC_SERIAL_STRING, string_descriptor, USB_MAX_STRING_DESCRIPTOR);
						buff->ptr = string_descriptor;
						break;
				default:
					return -1;
				}
				break;
			default:
				return -1;
		}
	}

	return 0;
}

struct usb_device usbd;

void init(void)
{
	delay_init();
#if CONFIG_HAS_CEC
	cec_init();
#endif
	usb_dev_init(&usbd);
	usbd.fops.sof = usb_cdc_sof;
	usbd.fops.get_device_descriptor = usb_cdc_get_device_descriptor;
}

int main(void)
{
	init();
	dbg("HDMI Snoop\n");
#if CONFIG_HAS_CEC
	memset(&cec_message_buffer, 0, sizeof(cec_message_buffer));
#endif
	int i=0;
	for(;;)
	{
#if CONFIG_HAS_CEC
		while(cec_message_buffer.size > 0)
		{
			struct cec_rx_message * msg = &cec_message_buffer.items[cec_message_buffer.first].message;
			cec_result_t result = cec_message_buffer.items[cec_message_buffer.first].result;
			up_time_t time = cec_message_buffer.items[cec_message_buffer.first].time;
			print_message(result, time, msg);
			cec_message_buffer.first = (cec_message_buffer.first+1)%CEC_BUFFER_SIZE;
			cec_message_buffer.size--;
		}
#endif
	}
	return 0;
}

void EXTI0_IRQHandler(void)
{
#if CONFIG_HAS_CEC
	static struct cec_rx_filter rxf = {
		.receive = {
			.device = {
				.STB1 = 1
			}
		},
		.monitor = {
			.initiator = {
				.all = CEC_RX_FILTER_ALL_DEVICES
			},
			.follower = {
				.all = CEC_RX_FILTER_ALL_DEVICES
			},
		}
	};
	if(EXTI->PR & CEC_EXTI_PR)
	{
		if(cec_message_buffer.size < CEC_BUFFER_SIZE)
		{
			struct cec_message_buffer_item * it = &cec_message_buffer.items[cec_message_buffer.last];
			struct cec_rx_message * msg = &it->message;
			it->result = cec_rx_message(msg, &rxf);
			it->time = get_up_time();
			cec_message_buffer.last = (cec_message_buffer.last + 1)%CEC_BUFFER_SIZE;
			cec_message_buffer.size++;
		}
		EXTI->PR = CEC_EXTI_PR;
	}
#endif
}

void print_message(cec_result_t result, up_time_t time, struct cec_rx_message * msg)
{
	if(CEC_RESULT_OK == result || CEC_ERROR_RX_DROPPED == result)
	{
		printf("| %02d:%02d:%02d.%03d | %02x -> %02x | L:%2d A:%d | ",
			time.hours,
			time.minutes,
			time.seconds,
			time.ms,
			msg->header.initiator,
			msg->header.follower,
			msg->length,
			msg->ack);

		for(int i=0;i<msg->length;i++)	
		{
			printf("0x%02x ", msg->message.buff[i]);
		}
		if(msg->length > 0)
		{
			printf("| <%s> ", cec_opcode_to_str(msg->message.data.opcode));
			if(CEC_OPCODE_ACTIVE_SOURCE == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.active_source.physical_address.A,
					msg->message.data.operand.active_source.physical_address.B,
					msg->message.data.operand.active_source.physical_address.C,
					msg->message.data.operand.active_source.physical_address.D
				      );
			}
			else if(CEC_OPCODE_ROUTING_INFORMATION == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.routing_information.physical_address.A,
					msg->message.data.operand.routing_information.physical_address.B,
					msg->message.data.operand.routing_information.physical_address.C,
					msg->message.data.operand.routing_information.physical_address.D
				      );

			}
			else if(CEC_OPCODE_SET_STREAM_PATH == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.set_stream_path.physical_address.A,
					msg->message.data.operand.set_stream_path.physical_address.B,
					msg->message.data.operand.set_stream_path.physical_address.C,
					msg->message.data.operand.set_stream_path.physical_address.D
				      );

			}
			else if(CEC_OPCODE_ROUTING_CHANGE == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d -> %d.%d.%d.%d] ", 
					msg->message.data.operand.routing_change.original_address.A,
					msg->message.data.operand.routing_change.original_address.B,
					msg->message.data.operand.routing_change.original_address.C,
					msg->message.data.operand.routing_change.original_address.D,
					msg->message.data.operand.routing_change.new_address.A,
					msg->message.data.operand.routing_change.new_address.B,
					msg->message.data.operand.routing_change.new_address.C,
					msg->message.data.operand.routing_change.new_address.D
				      );

			}
			else if(CEC_OPCODE_REPORT_PHYSICAL_ADDRESS == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] [%s] ", 
					msg->message.data.operand.report_physical_address.physical_address.A,
					msg->message.data.operand.report_physical_address.physical_address.B,
					msg->message.data.operand.report_physical_address.physical_address.C,
					msg->message.data.operand.report_physical_address.physical_address.D,
					cec_device_type_to_str(msg->message.data.operand.report_physical_address.device_type)
				      );

			}
			else if(CEC_OPCODE_REPORT_POWER_STATUS == msg->message.data.opcode)
			{
				printf("[%s] ", cec_power_status_to_str(msg->message.data.operand.power_status));
			}
			else if(CEC_OPCODE_MENU_REQUEST == msg->message.data.opcode)
			{
				printf("[%s] ", cec_menu_request_type_to_str(msg->message.data.operand.menu_request_type));
			}
			else if(CEC_OPCODE_MENU_STATUS == msg->message.data.opcode)
			{
				printf("[%s] ", cec_menu_state_to_str(msg->message.data.operand.menu_state));
			}
			else if(CEC_OPCODE_USER_CONTROL_PRESSED == msg->message.data.opcode)
			{
				printf("[%s] ", cec_user_control_to_str(msg->message.data.operand.user_control_pressed));
			}
			else if(CEC_OPCODE_SET_OSD_NAME == msg->message.data.opcode)
			{
				msg->message.data.operand.buff[msg->length-1] = '\0';
				printf("[%s] ", (const char*)msg->message.data.operand.buff);
			}
			else if(CEC_OPCODE_SET_OSD_STRING == msg->message.data.opcode)
			{
				msg->message.data.operand.set_osd_string.string[msg->length-2] = '\0';
				printf("[%s] [%s] ", 
					cec_display_control_to_str(msg->message.data.operand.set_osd_string.display_control),
					(const char*)msg->message.data.operand.set_osd_string.string
				);

			}
		}
		if(CEC_ERROR_RX_DROPPED == result)
		{
			printf(" -> message dropped");
		}
		printf("\n");
	}
	else
	{
		printf("| %02d:%02d:%02d.%03d | %02x -> %02x | L:%2d A:%d | %s\n",
			time.hours,
			time.minutes,
			time.seconds,
			time.ms,
			msg->header.initiator,
			msg->header.follower,
			msg->length,
			msg->ack,
			cec_result_to_str(result)
		      );
	}
}

