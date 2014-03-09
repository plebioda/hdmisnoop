#ifndef _USB_H
#define _USB_H

#include <stdint.h>

typedef uint32_t usb_ret_t;

typedef enum 
{
	USB_ENDPOINT_TYPE_CONTROL = 0,
	USB_ENDPOINT_TYPE_ISOCHRONOUS,
	USB_ENDPOINT_TYPE_BULK,
	USB_ENDPOINT_TYPE_INTERRUPT
} usb_endpoint_type_t;

typedef enum
{
	USB_ENDPOINT_DIR_OUT,
	USB_ENDPOINT_DIR_IN
} usb_endpoint_dir_t;

typedef enum
{
	USB_SPEED_LOW,
	USB_SPEED_FULL,
	USB_SPEED_HIGH,

	USB_SPEED_MAX
} usb_speed_t;

typedef enum
{
	USB_MODE_DEVICE,
	USB_MODE_HOST,

	USB_MODE_MAX
} usb_mode_t;

typedef enum
{
	USB_DEVICE_STATE_ATTACHED,
	USB_DEVICE_STATE_POWERED,
	USB_DEVICE_STATE_SUSPENDED,
	USB_DEVICE_STATE_DEFAULT,
	USB_DEVICE_STATE_ADDRESS,
	USB_DEVICE_STATE_CONFIGURED
} usb_device_state_t;

typedef enum
{
	USB_REQUEST_TYPE_STANDARD = 0,
	USB_REQUEST_TYPE_CLASS = 1,
	USB_REQUEST_TYPE_VENDOR = 2,
	USB_REQUEST_TYPE_MAX
} usb_request_type_t;

typedef enum
{
	USB_REQUEST_RECIPIENT_DEVICE = 0,
	USB_REQUEST_RECIPIENT_INTERFACE = 1,
	USB_REQUEST_RECIPIENT_ENPOINT = 2,
	USB_REQUEST_RECIPIENT_OTHER = 3,
	USB_REQUEST_RECIPIENT_MAX
} usb_request_recipient_t;

typedef enum
{
	USB_REQUEST_DIRECTION_HOST_TO_DEVICE = 0,
	USB_REQUEST_DIRECTION_DEVICE_TO_HOST = 1
} usb_request_direction_t;

typedef enum
{
	USB_REQUEST_GET_STATUS = 0,
	USB_REQUEST_CLEAR_FEATURE = 1,
	USB_REQUEST_RESERVED_2 = 2,
	USB_REQUEST_SET_FEATURE = 3,
	USB_REQUEST_RESERVED_4 = 4,
	USB_REQUEST_SET_ADDRESS = 5,
	USB_REQUEST_GET_DESCRIPTOR = 6,
	USB_REQUEST_SET_DESCRIPTOR = 7,
	USB_REQUEST_GET_CONFIGURATION = 8,
	USB_REQUEST_SET_CONFIGURATION = 9,
	USB_REQUEST_GET_INTERFACE = 10,
	USB_REQUEST_SET_INTERFACE = 11,
	USB_REQUEST_SYNCH_FRAME = 12,
	USB_REQUEST_MAX
} usb_request_t;

typedef enum
{
	USB_DESCRIPTOR_TYPE_DEVICE = 1,
	USB_DESCRIPTOR_TYPE_CONFIGURATION = 2,
	USB_DESCRIPTOR_TYPE_STRING = 3,
	USB_DESCRIPTOR_TYPE_INTERFACE = 4,
	USB_DESCRIPTOR_TYPE_ENDPOINT = 5,
	USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 6,
	USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION = 7,
	USB_DESCRIPTOR_TYPE_INTERFACE_POWER = 8,
	USB_DESCRIPTOR_TYPE_MAX
} usb_descriptor_type_t;

typedef enum
{
	USB_FEATURE_SELECTOR_ENDPOINT_HALT = 0,
	USB_FEATURE_SELECTOR_DEVICE_REMOTE_WAKEUP = 1,
	USB_FEATURE_SELECTOR_TEST_MODE = 2,
	USB_FEATURE_SELECTOR_MAX
} usb_feature_selector_t;

typedef uint16_t usb_address_t;
#define USB_ADDRESS_MASK	0x7f

typedef struct usb_setup_packet
{
	union
	{
		uint8_t val;
		struct
		{
			usb_request_recipient_t Recipient	: 5;
			usb_request_type_t Type			: 2;
			usb_request_direction_t Direction	: 1;
		} b;
	} bmRequestType;
	uint8_t bRequest;
	union
	{
		uint16_t val;
		struct
		{
			uint8_t index;
			usb_descriptor_type_t type;
		} desc;
		usb_address_t address;
	} wValue;
	uint16_t wIndex;
	uint16_t wLength;
} PACKED usb_setup_packet_t;


typedef struct usb_device_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bcdUSB[2];
	uint8_t bDeviceClass;
	uint8_t bDeviceSubClass;
	uint8_t bDeviceProtocol;
	uint8_t bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint8_t bcdDevice[2];
	uint8_t iManufacturer;
	uint8_t iProduct;
	uint8_t iSerialNumber;
	uint8_t bNumConfigurations;
} PACKED usb_device_descriptor_t;

typedef struct usb_configuration_descritor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	struct 
	{
		uint8_t reserved4_0	: 5;
		uint8_t RemoteWakeup	: 1;
		uint8_t SelfPowered	: 1;
		uint8_t reserved7	: 1;
	} bmAttributes;
	uint8_t bMaxPower;
} PACKED usb_configuration_descriptor_t;

typedef struct usb_interface_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} PACKED usb_interface_descriptor_t;

typedef struct usb_endpoint_address
{
	uint8_t number		: 4;
	uint8_t reserved6_4 	: 3;
	usb_endpoint_dir_t dir	: 1;
} PACKED usb_endpoint_address_t;

typedef enum
{
	USB_ENDPOINT_SYNC_TYPE_NO = 0,
	USB_ENDPOINT_SYNC_TYPE_ASYNC,
	USB_ENDPOINT_SYNC_TYPE_ADAPTIVE,
	USB_ENDPOINT_SYNC_TYPE_SYNC
} usb_endpoint_sync_type_t;

typedef enum
{
	USB_ENDPOINT_USAGE_TYPE_DATA = 0,
	USB_ENDPOINT_USAGE_TYPE_FEEDBACK,
	USB_ENDPOINT_USAGE_TYPE_IMPLICIT_FEEDBACK_DATA,
	USB_ENDPOINT_USAGE_TYPE_RESERVED,

} usb_endpoint_usage_type_t;

typedef struct usb_endpoint_descriptor
{
	uint8_t bLength;
	uint8_t bDescriptorType;
	usb_endpoint_address_t bEndpointAddress;
	struct
	{
		usb_endpoint_type_t TransferType    : 2;
		usb_endpoint_sync_type_t SyncType   : 2;
		usb_endpoint_usage_type_t UsageType : 2;
		uint8_t reserved7_6                 : 2;
	} bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} PACKED usb_endpoint_descriptor_t;

#endif //_USB_H
