#ifndef _USB_H
#define _USB_H

#include <stdint.h>

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
	}wValue;
	uint16_t wIndex;
	uint16_t wLength;
} usb_setup_packet_t;

#endif //_USB_H
