#ifndef _USB_DBG_H
#define _USB_DBG_H

#include <usb.h>

const char * usb_request_type_to_str(usb_request_type_t type);
const char * usb_request_recipient_to_str(usb_request_recipient_t rec);
const char * usb_request_direction_to_str(usb_request_direction_t dir);
const char * usb_request_to_str(usb_request_t req);
const char * usb_descriptor_type_to_str(usb_descriptor_type_t type);

#endif //_USB_DBG_H
