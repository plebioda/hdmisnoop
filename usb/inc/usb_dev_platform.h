#ifndef _USB_DEV_PLATFORM_H
#define _USB_DEV_PLATFORM_H

#include <usb.h>
#include <usb_dev_endpoint.h>

struct usb_dev_platform_callbacks
{
	void * context;
	void (*enum_done)(void * context, usb_speed_t speed);
	void (*reset)(void * context);
	void (*setup_done)(void * context, usb_setup_packet_t * setup_packet);
	void (*sof)(void * context);
	void (*tx_fifo_empty)(void * context, uint32_t n);
	void (*tx_completed)(void * context, uint32_t n);
};

struct usb_dev_platform;

typedef struct usb_dev_platform * usb_dev_platform_handle_t;

usb_dev_platform_handle_t usb_dev_platform_init(struct usb_dev_platform_callbacks * callbacks);
int usb_dev_platform_ep0_tx_start(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);
int usb_dev_platform_ep_activate(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);
int usb_dev_platform_write_fifo(usb_dev_platform_handle_t handle, struct usb_device_endpoint * ep);
int usb_dev_platform_set_address(usb_dev_platform_handle_t handle, usb_address_t addr);
int usb_dev_platform_rx_prepare(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep);
int usb_dev_platform_rx_setup(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep);
int usb_dev_platform_ep_stall(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep);

#endif //_USB_DEV_PLATFORM_H
