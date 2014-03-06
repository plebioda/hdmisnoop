#include <stdlib.h>
#include <string.h>
#include <usb_dev.h>
#include <usb_dev_platform.h>
#include <debug.h>
#include <usb_dbg.h>

int usb_dev_ep_tx(struct usb_device * usbd, uint8_t n, uint8_t * buff, uint32_t len)
{
	struct usb_device_endpoint * ep = &usbd->ep_in[n];
	ep->id = n;
	ep->buff.ptr = buff;
	ep->buff.len = len;

	if(ep->id == 0)
	{
		usb_dev_platform_ep0_tx_start(usbd->platform, ep);
	}
	else
	{
	}
}

int usb_dev_ep_rx(struct usb_device * usbd)
{
	TODO("Implement me\n");
}

int usb_dev_ctl_send_data(struct usb_device * usbd, uint8_t * buff, uint32_t len)
{
	return usb_dev_ep_tx(usbd, 0, buff, len);
}

int usb_dev_ctl_req_error(struct usb_device * usbd, struct usb_setup_packet * req)
{
	TODO("Implement me\n");
}

int usb_dev_ctl_send_status(struct usb_device * usbd)
{
	usb_dev_ep_tx(usbd, 0, NULL, 0);
	
	usb_dev_ep_rx(usbd);

	return 0;
}

int usb_dev_request_set_address(struct usb_device * usbd, struct usb_setup_packet * setup_packet)
{
	if(USB_REQUEST_DIRECTION_HOST_TO_DEVICE != setup_packet->bmRequestType.b.Direction)
	{
		dbg("Invalid direction of SET_ADDRESS\n");
		return -1;
	}

	if(setup_packet->wIndex || setup_packet->wLength || USB_DEVICE_STATE_CONFIGURED == usbd->state)
	{
		dbg("Invalid data of SET_ADDRESS\n");
		usb_dev_ctl_req_error(usbd, setup_packet);
		return -1;
	}

	usb_address_t addr = (usb_address_t)(setup_packet->wValue.address&USB_ADDRESS_MASK);
	
	dbg("addr = %d\n", addr);

	if(addr && !usb_dev_platform_set_address(usbd->platform, addr))
	{
		usbd->address = addr;
		usbd->state = USB_DEVICE_STATE_ADDRESS;
		dbg("USB Device Address: %d\n", addr);
		return usb_dev_ctl_send_status(usbd);
	}
	else
	{
		usbd->address = 0;
		usbd->state = USB_DEVICE_STATE_DEFAULT;

		return -1;
	}
}

int usb_dev_request_get_descriptor(struct usb_device * usbd, struct usb_setup_packet * setup_packet)
{
	uint8_t * buff = NULL;
	uint32_t len = 0;
	
	if(USB_REQUEST_DIRECTION_DEVICE_TO_HOST != setup_packet->bmRequestType.b.Direction)
	{
		return -1;
	}
	
	switch(setup_packet->wValue.desc.type)
	{
		case USB_DESCRIPTOR_TYPE_DEVICE:
			if(usbd->fops.get_device_descriptor)
			{
				usb_ret_t ret = usbd->fops.get_device_descriptor(usbd, &buff, &len);
				if(ret)
				{
					return -1;
				}
				if(setup_packet->wLength == 64 && USB_DEVICE_STATE_DEFAULT == usbd->state)
				{
					len = 8;
				}
			}
			break;
		case USB_DESCRIPTOR_TYPE_CONFIGURATION:
		case USB_DESCRIPTOR_TYPE_STRING:
		case USB_DESCRIPTOR_TYPE_INTERFACE:
		case USB_DESCRIPTOR_TYPE_ENDPOINT:
		case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
		case USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
		case USB_DESCRIPTOR_TYPE_INTERFACE_POWER:
		default:
			return -1;
	}
	if(NULL != buff && len>0 && setup_packet->wLength>0)
	{
		return usb_dev_ctl_send_data(usbd, buff, len);
	}
	else
	{
		return -1;
	}
}


int usb_dev_request_device(struct usb_device * usbd, struct usb_setup_packet * setup_packet)
{
	usb_request_t req = (usb_request_t)setup_packet->bRequest;
	switch(req)
	{
		case USB_REQUEST_GET_STATUS:
		case USB_REQUEST_CLEAR_FEATURE:
		case USB_REQUEST_SET_FEATURE:
			return -1;
		case USB_REQUEST_SET_ADDRESS:
			return usb_dev_request_set_address(usbd, setup_packet);
		case USB_REQUEST_GET_DESCRIPTOR:
			return usb_dev_request_get_descriptor(usbd, setup_packet);
		case USB_REQUEST_SET_DESCRIPTOR:
		case USB_REQUEST_GET_CONFIGURATION:
		case USB_REQUEST_SET_CONFIGURATION:
		case USB_REQUEST_GET_INTERFACE:
		case USB_REQUEST_SET_INTERFACE:
		case USB_REQUEST_SYNCH_FRAME:
		case USB_REQUEST_RESERVED_4:
		case USB_REQUEST_RESERVED_2:
		default:
			return -1;
	}
}

int usb_dev_setup_done_irq(struct usb_device * usbd, struct usb_setup_packet * setup_packet)
{
	usb_dbg_print_setup_packet(setup_packet);
	if(USB_REQUEST_TYPE_STANDARD == setup_packet->bmRequestType.b.Type)
	{
		switch(setup_packet->bmRequestType.b.Recipient)
		{
			case USB_REQUEST_RECIPIENT_DEVICE:
				return usb_dev_request_device(usbd, setup_packet);
			case USB_REQUEST_RECIPIENT_INTERFACE:
				break;
			case USB_REQUEST_RECIPIENT_ENPOINT:
				break;
			case USB_REQUEST_RECIPIENT_OTHER:
				break;
			default:
				return -1;	
		}
	}
	else
	{
		return -1;
	}
}


int usb_dev_ep_open(struct usb_device * usbd, usb_endpoint_dir_t dir, uint8_t id, usb_endpoint_type_t type, uint16_t mps)
{
	struct usb_device_endpoint * ep = NULL;

	if(USB_ENDPOINT_DIR_IN == dir)
	{
		ep = &usbd->ep_in[id];
	}
	else
	{
		ep = &usbd->ep_out[id];
	}

	ep->dir = dir;
	ep->type = type;
	ep->mps = mps;

	return usb_dev_platform_ep_activate(usbd->platform, ep);
}

void usb_dev_platform_callback_tx_fifo_empty(void * context, uint32_t n)
{
	if(NULL != context)
	{
		struct usb_device * usbd = (struct usb_device*)context;
		
		if(n < USB_DEVICE_IN_ENDPOINTS_NUMBER)
		{
			usbd->ep_in[n].id = n;
			usb_dev_platform_write_fifo(usbd->platform, &usbd->ep_in[n]);
		}
	}
}

void usb_dev_platform_callback_sof(void * context)
{
	if(NULL != context)
	{
		struct usb_device * usbd = (struct usb_device*)context;
		if(usbd->fops.sof)
		{
			usbd->fops.sof(usbd);
		}
	}
}

void usb_dev_platform_callback_enum_done(void * context, usb_speed_t speed)
{
	if(NULL != context)
	{
		struct usb_device * usbd = (struct usb_device*)context;
		usbd->config.speed = speed;
	}
}

void usb_dev_platform_callback_setup_done(void * context, struct usb_setup_packet * setup_packet)
{
	if(NULL != context && NULL != setup_packet)
	{
		struct usb_device * usbd = (struct usb_device*)context;
	
		usb_dev_setup_done_irq(usbd, setup_packet);
	}
}


void usb_dev_platform_callback_reset(void * context)
{
#define USB_EP0_MPS		64
	if(NULL != context)
	{
		struct usb_device * usbd = (struct usb_device*)context;
		
		usb_dev_ep_open(usbd, USB_ENDPOINT_DIR_OUT, 0, USB_ENDPOINT_TYPE_CONTROL, USB_EP0_MPS);
		usb_dev_ep_open(usbd, USB_ENDPOINT_DIR_IN, 0, USB_ENDPOINT_TYPE_CONTROL, USB_EP0_MPS);
		
		usbd->state = USB_DEVICE_STATE_DEFAULT;
	}
}



int usb_dev_init(struct usb_device * usbd)
{
	int ret=0;
	memset(usbd, 0, sizeof(struct usb_device));
	
	for(int i=0;i<USB_DEVICE_IN_ENDPOINTS_NUMBER;i++)
	{
		usbd->ep_in[i].id = i;
	}
	
	for(int i=0;i<USB_DEVICE_OUT_ENDPOINTS_NUMBER;i++)
	{
		usbd->ep_out[i].id = i;
	}


	usbd->platform_callbacks.context = usbd;
	usbd->platform_callbacks.enum_done = usb_dev_platform_callback_enum_done;
	usbd->platform_callbacks.reset = usb_dev_platform_callback_reset;
	usbd->platform_callbacks.sof = usb_dev_platform_callback_sof;
	usbd->platform_callbacks.tx_fifo_empty = usb_dev_platform_callback_tx_fifo_empty;
	usbd->platform_callbacks.setup_done = usb_dev_platform_callback_setup_done;

	usb_dev_platform_handle_t handle = usb_dev_platform_init(&usbd->platform_callbacks);

	if(NULL != handle)
	{
		usbd->platform = handle;
	}
	else
	{
		return -1;
	}
}


