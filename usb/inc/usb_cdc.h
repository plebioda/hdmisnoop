#ifndef _USB_CDC_H
#define _USB_CDC_H

typedef enum
{
	USB_CDC_DESCRIPTOR_TYPE_CS_INTERFACE = 0x24,
	USB_CDC_DESCRIPTOR_TYPE_CS_ENDPOINT = 0x25
} usb_cdc_descriptor_type_t;

typedef enum
{
	USB_CDC_DESCRIPTOR_SUBTYPE_HEADER = 0,
	USB_CDC_DESCRIPTOR_SUBTYPE_CALL_MANAGEMENT,
	USB_CDC_DESCRIPTOR_SUBTYPE_ABSTRACT_CONTROL_MANAGEMENT,
	USB_CDC_DESCRIPTOR_SUBTYPE_DIRECT_LINE_MANAGEMENT,
	USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_RINGER,
	USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_CALL_LINE_STATE_REPORTINB_CAPABILITIES,
	USB_CDC_DESCRIPTOR_SUBTYPE_UNION,
	USB_CDC_DESCRIPTOR_SUBTYPE_COUNTRY_SELECTION,
	USB_CDC_DESCRIPTOR_SUBTYPE_TELEPHONE_OPERATIONAL_MODES,
	USB_CDC_DESCRIPTOR_SUBTYPE_USB_TERMINAL,
	USB_CDC_DESCRIPTOR_SUBTYPE_NETWORK_CHANNEL_TERMINAL,
	USB_CDC_DESCRIPTOR_SUBTYPE_PROTOCOL_UNIT,
	USB_CDC_DESCRIPTOR_SUBTYPE_EXTENSION_UNIT,
	USB_CDC_DESCRIPTOR_SUBTYPE_MULTICHANNEL_MANAGEMENT,
	USB_CDC_DESCRIPTOR_SUBTYPE_CAPI_CONTROL_MANAGEMENT,
	USB_CDC_DESCRIPTOR_SUBTYPE_ETHERNET_NETWORKING,
	USB_CDC_DESCRIPTOR_SUBTYPE_ATM_NETWORKING
} usb_cdc_descriptor_subtype_t;

/*
 * Header Functional Descriptor
 */
typedef struct usb_cdc_header_descriptor
{
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bcdCDC[2];

} usb_cdc_header_descriptor_t;

/*
 * Call Management Functional Descriptor
 */
typedef struct usb_cdc_cm_descriptor
{
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
	uint8_t bDataInterface;
} usb_cdc_cm_descriptor_t;

/*
 * Abstract Control Management Functional Descriptor
 */
typedef struct usb_cdc_acm_descriptor
{
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
} usb_cdc_acm_descriptor_t;

/*
 * Direct Line Management Functional Descriptor
 */
typedef struct usb_cdc_dlm_descriptor
{
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bmCapabilities;
} usb_cdc_dlm_descriptor_t;

/*
 * Union Functional Descriptor
 */
typedef struct usb_cdc_union_descriptor
{
	uint8_t bFunctionLength;
	uint8_t bDescriptorType;
	uint8_t bDescriptorSubtype;
	uint8_t bMasterInterface;
} usb_cdc_union_descriptor_t;

#endif //_USB_CDC_H
