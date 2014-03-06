#include <usb_dbg.h>
#include <debug.h>

void usb_dbg_print_setup_packet(struct usb_setup_packet * p)
{
	dbg("SETUP:\n");
	dbg("bmRequestType.Direction : %d [%s]\n", p->bmRequestType.b.Direction, usb_request_direction_to_str(p->bmRequestType.b.Direction));
	dbg("bmRequestType.Type      : %d [%s]\n", p->bmRequestType.b.Type, usb_request_type_to_str(p->bmRequestType.b.Type));
	dbg("bmRequestType.Recipient : %d [%s]\n", p->bmRequestType.b.Recipient, usb_request_recipient_to_str(p->bmRequestType.b.Recipient));
	dbg("bRequest                : %d [%s]\n", p->bRequest, usb_request_to_str(p->bRequest));
	if(USB_REQUEST_GET_DESCRIPTOR == p->bRequest)
	{
		dbg("wValue                  : %d [%s(%d)]\n", p->wValue.val, usb_descriptor_type_to_str(p->wValue.desc.type), p->wValue.desc.index);
	}
	else
	{
		dbg("wValue                  : %d\n", p->wValue.val);
	}
	dbg("wIndex                  : %d\n", p->wIndex);
	dbg("wLength                 : %d\n", p->wLength);
}

const char * usb_request_type_to_str(usb_request_type_t type)
{
	switch(type)
	{
		case USB_REQUEST_TYPE_STANDARD:
			return "Standard";
		case USB_REQUEST_TYPE_CLASS:
			return "Class";
		case USB_REQUEST_TYPE_VENDOR:
			return "Vendor";
		default:
			return "Reserved";
	}
}

const char * usb_request_recipient_to_str(usb_request_recipient_t rec)
{
	switch(rec)
	{
		case USB_REQUEST_RECIPIENT_DEVICE:
			return "Device";
		case USB_REQUEST_RECIPIENT_INTERFACE:
			return "Interface";
		case USB_REQUEST_RECIPIENT_ENPOINT:
			return "Endpoint";
		case USB_REQUEST_RECIPIENT_OTHER:
			return "Other";
		default:
			return "Unknown";
	}
}

const char * usb_request_direction_to_str(usb_request_direction_t dir)
{
	switch(dir)
	{
		case USB_REQUEST_DIRECTION_HOST_TO_DEVICE:
			return "Host to Device";
		case USB_REQUEST_DIRECTION_DEVICE_TO_HOST:
			return "Device to Host";
		default: 
			return "Unknown";
		
	}
}

const char * usb_request_to_str(usb_request_t req)
{
	switch(req)
	{
		case USB_REQUEST_GET_STATUS:
			return "GET_STATUS";
		case USB_REQUEST_CLEAR_FEATURE:
			return "CLEAR_FEATURE";
		case USB_REQUEST_SET_FEATURE:
			return "SET_FEATURE";
		case USB_REQUEST_SET_ADDRESS:
			return "SET_ADDRESS";
		case USB_REQUEST_GET_DESCRIPTOR:
			return "GET_DESCRIPTOR";
		case USB_REQUEST_SET_DESCRIPTOR:
			return "SET_DESCRIPTOR";
		case USB_REQUEST_GET_CONFIGURATION:
			return "GET_CONFIGURATION";
		case USB_REQUEST_SET_CONFIGURATION:
			return "SET_CONFIGURATION";
		case USB_REQUEST_GET_INTERFACE:
			return "GET_INTERFACE";
		case USB_REQUEST_SET_INTERFACE:
			return "SET_INTERFACE";
		case USB_REQUEST_SYNCH_FRAME:
			return "SYNCH_FRAME";
		case USB_REQUEST_RESERVED_4:
		case USB_REQUEST_RESERVED_2:
			return "RESERVED";
		default:
			return "Unknown";
	}
}

const char * usb_descriptor_type_to_str(usb_descriptor_type_t type)
{
	switch(type)
	{
		case USB_DESCRIPTOR_TYPE_DEVICE:
			return "DEVICE";
		case USB_DESCRIPTOR_TYPE_CONFIGURATION:
			return "CONFIGURATION";
		case USB_DESCRIPTOR_TYPE_STRING:
			return "STRING";
		case USB_DESCRIPTOR_TYPE_INTERFACE:
			return "INTERFACE";
		case USB_DESCRIPTOR_TYPE_ENDPOINT:
			return "ENDPOINT";
		case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
			return "DEVICE_QUALIFIER";
		case USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIGURATION:
			return "OTHER_SPEED_CONFIGURATION";
		case USB_DESCRIPTOR_TYPE_INTERFACE_POWER:
			return "INTERFACE_POWER";
		default:
			return "Unknown";
	
	}
}

