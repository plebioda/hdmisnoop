#ifndef _USB_ENDPOINT_H
#define _USB_ENDPOINT_H

struct usb_device_buffer
{
	uint8_t * ptr;
	uint32_t len;
};

struct usb_device_endpoint
{
	uint8_t id;
	usb_endpoint_dir_t dir;
	usb_endpoint_type_t type;
	struct usb_device_buffer buff;
	uint16_t mps;
	uint8_t stall;
};



#endif //_USB_ENDPOINT_H
