#ifndef _USB_DEV_PLATFORM_H
#define _USB_DEV_PLATFORM_H

#include <usb.h>

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
};

struct usb_dev_platform_callbacks
{
	void * context;
	void (*enum_done)(void * context, usb_speed_t speed);
	void (*reset)(void * context);
	void (*setup_done)(void * context, usb_setup_packet_t * setup_packet);
	void (*sof)(void * context);
	void (*tx_fifo_empty)(void * context, uint32_t n);
};

struct usb_dev_platform;

typedef struct usb_dev_platform * usb_dev_platform_handle_t;

usb_dev_platform_handle_t usb_dev_platform_init(struct usb_dev_platform_callbacks * callbacks);
int usb_dev_platform_ep0_tx_start(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);
int usb_dev_platform_ep_activate(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);
int usb_dev_platform_write_fifo(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);

#endif //_USB_DEV_PLATFORM_H
