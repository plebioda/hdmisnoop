#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f4xx.h>
#include <cec.h>
#include <clock.h>
#include <stm32_usb_fs.h>
#include <usb.h>
#include <usb_dbg.h>

#define err(fmt, args...)	printf("error: " fmt, ## args)
#define dbg(fmt, args...)	{						\
					up_time_t time = get_up_time();		\
					printf("[%02d:%02d:%02d.%03d] debug: " fmt, time.hours, time.minutes, time.seconds, time.ms, ## args);		\
				}
#define STR(name)	#name
#define PRINT_FIELD(n, f)	dbg("%s.%s	: %d\n", STR(n), STR(f), n.b.f)

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

int usb_init_io(void)
{
	/*
	 * Ports initialization
	 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	/*
	 * PA8 - USB OTG FS SOF
	 */
	GPIOA->MODER &=~(GPIO_MODER_MODER8);
	/* Alternate Function */
	GPIOA->MODER |= (GPIO_MODER_MODER8_1);
	/* Push Pull */
	GPIOA->OTYPER &=~(GPIO_OTYPER_OT_8);
	/* No pull-up, no pull-down */
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR8);
	/* Speed */
	GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR8);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8_1);

	GPIOA->AFR[1] &= ~0xf;
	GPIOA->AFR[1] |= 0xa;

	/*
	 *  PA9 - USB OTG FS VBUS
	 */
	/* Input */
	GPIOA->MODER &=~(GPIO_MODER_MODER9);
	/* Open drain */
	GPIOA->OTYPER |= (GPIO_OTYPER_OT_9);
	/* No pull-up, no pull-down */
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR9);
	/* Speed */
	GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR9);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_1);

	/*
	 *  PA10 - USB OTG FS ID
	 */
	/* Input */
	GPIOA->MODER &=~(GPIO_MODER_MODER10);
	/* Open drain */
	GPIOA->OTYPER |= (GPIO_OTYPER_OT_10);
	/* Pull-up*/
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR10);
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR10_0);
	/* Speed */
	GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR10);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10_1);

	/*
	 *  PA11 - USB OTG FS DM
	 */
	GPIOA->MODER &=~(GPIO_MODER_MODER11);
	/* Alternate Function */
	GPIOA->MODER |= (GPIO_MODER_MODER11_1);
	/* Push Pull */
	GPIOA->OTYPER &=~(GPIO_OTYPER_OT_11);
	/* No pull-up, no pull-down */
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR11);
	/* Speed */
	GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR11);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR11_1);

	GPIOA->AFR[1] &= ~0xf000;
	GPIOA->AFR[1] |= 0xa000;

	/*
	 *  PA12 - USB OTG FS DP
	 */
	GPIOA->MODER &=~(GPIO_MODER_MODER12);
	/* Alternate Function */
	GPIOA->MODER |= (GPIO_MODER_MODER12_1);
	/* Push Pull */
	GPIOA->OTYPER &=~(GPIO_OTYPER_OT_12);
	/* No pull-up, no pull-down */
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR12);
	/* Speed */
	GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR12);
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR12_1);

	GPIOA->AFR[1] &= ~0xf0000;
	GPIOA->AFR[1] |= 0xa0000;
	
	RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

	return 0;
}

int usb_init_irq(void)
{
	NVIC_SetPriority(OTG_FS_IRQn, 2);
	NVIC_EnableIRQ(OTG_FS_IRQn);

	return 0;
}

int usb_init_core(void)
{
	USB_OTG_FS_GAHBCFG_T ahbcfg;
	USB_OTG_FS_GUSBCFG_T usbcfg;
	USB_OTG_FS_GCCFG_T gccfg;

	/* Disable global Interrupt */
	ahbcfg.reg = 0;
	ahbcfg.b.GINTMSK = 1;
	
	USB_OTG_FS_CGCSR->GAHBCFG = ahbcfg;
	
	/*
	 * Core Init
	 */
	printf("Core init...\n");
	ahbcfg.reg = 0;
	usbcfg.reg = 0;
	gccfg.reg = 0;

	usbcfg = USB_OTG_FS_CGCSR->GUSBCFG;
	usbcfg.b.PHYSEL = 1;
	USB_OTG_FS_CGCSR->GUSBCFG = usbcfg;
	
	int counter=0;
	/*
	 * Core reset
	 */
	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			printf("AHBIDL delay\n");
			return -1;
		}
	} while(USB_OTG_FS_CGCSR->GRSTCTL.b.AHBIDL == 0);

	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			printf("CSRST delay\n");
			return -1;
		}
	} while(USB_OTG_FS_CGCSR->GRSTCTL.b.CSRST == 1);

	delay_us(100);

	/*
	 * Deactivate the power down
	 */
	gccfg.reg = 0;
	gccfg.b.PWRDWN = 1;
	gccfg.b.VBUSASEN = 1;
	gccfg.b.VBUSBSEN = 1;
	USB_OTG_FS_CGCSR->GCCFG = gccfg;
	delay_us(20000);

	/*
	 * Force device mode
	 */
	usbcfg = USB_OTG_FS_CGCSR->GUSBCFG;
	usbcfg.b.FDMOD = 1;
	USB_OTG_FS_CGCSR->GUSBCFG = usbcfg;
	delay_us(50000);

	/*
	 * Init device
	 */
	/* Restart PHY clock */
	USB_OTG_FS_PCGCCTL->reg = 0;

	USB_OTG_FS_DCFG_T dcfg;

	dcfg = USB_OTG_FS_DMCSR->DCFG;
	dcfg.b.PFIVL = 0;
	/* Full speed */
	dcfg.b.DSPD = 3;
	USB_OTG_FS_DMCSR->DCFG = dcfg;
	
	USB_OTG_FS_CGCSR->GRXFSIZ.b.RXFD = 128;

	/* EP0 TX */
	USB_OTG_FS_DIEPTXF0_T dieptxf0;
	dieptxf0.d.TX0FD = 32;
	dieptxf0.d.TX0FSA = 128;
	USB_OTG_FS_CGCSR->DIEPTXF0 = dieptxf0;

	/* EP1 TX */
	USB_OTG_FS_DIEPTXFx_T dieptxf1;
	dieptxf1.b.INEPTXSA = dieptxf0.d.TX0FSA + dieptxf0.d.TX0FD; 
	dieptxf1.b.INEPTXFD = 128;
	USB_OTG_FS_CGCSR->DIEPTXFx[0] = dieptxf1;
	
	/* EP2 TX */
	USB_OTG_FS_DIEPTXFx_T dieptxf2;
	dieptxf2.b.INEPTXSA = dieptxf1.b.INEPTXSA + dieptxf1.b.INEPTXFD; 
	dieptxf2.b.INEPTXFD = 32;
	USB_OTG_FS_CGCSR->DIEPTXFx[1] = dieptxf2;

	/* EP3 TX */
	USB_OTG_FS_DIEPTXFx_T dieptxf3;
	dieptxf3.b.INEPTXSA = dieptxf2.b.INEPTXSA + dieptxf2.b.INEPTXFD; 
	dieptxf3.b.INEPTXFD = 32;
	USB_OTG_FS_CGCSR->DIEPTXFx[1] = dieptxf3;

	USB_OTG_FS_GRSTCTL_T grstctl;

	/* Flush TX FIFO */
	grstctl.reg = 0;
	grstctl.b.TXFFLSH = 1;
	grstctl.b.TXFNUM = 0x10;
	USB_OTG_FS_CGCSR->GRSTCTL = grstctl;
	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			printf("TX FIFO flush delay\n");
			return -1;
		}

	} while(USB_OTG_FS_CGCSR->GRSTCTL.b.TXFFLSH == 1);
	delay_us(100);
	
	/* Flush RX FIFO */
	grstctl.reg = 0;
	grstctl.b.RXFFLSH = 1;
	USB_OTG_FS_CGCSR->GRSTCTL = grstctl;
	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			printf("RX FIFO flush delay\n");
			return -1;
		}

	} while(USB_OTG_FS_CGCSR->GRSTCTL.b.RXFFLSH == 1);
	delay_us(100);

	/* Clear all pending interrupts */
	USB_OTG_FS_DMCSR->DIEPMSK.reg = 0;
	USB_OTG_FS_DMCSR->DOEPMSK.reg = 0;
	USB_OTG_FS_DMCSR->DAINT.reg = 0xffffffff;
	USB_OTG_FS_DMCSR->DAINTMSK.reg = 0;

	for(int i=0;i<USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		USB_OTG_FS_DIEPCTL(i)->reg = 0;
		USB_OTG_FS_DIEPSIZ(i)->reg = 0;
		USB_OTG_FS_DIEPINT(i)->reg = 0xFF;
	}
	
	for(int i=0;i<USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		USB_OTG_FS_DOEPCTL(i)->reg = 0;
		USB_OTG_FS_DOEPSIZ(i)->reg = 0;
		USB_OTG_FS_DOEPINT(i)->reg = 0xFF;
	}

	USB_OTG_FS_GINTMSK_T intmsk;
	intmsk.reg = 0;
	USB_OTG_FS_CGCSR->GINTMSK.reg = 0;
	//USB_OTG_FS_CGCSR->GOTGINT.reg = 0xffffffff;
	USB_OTG_FS_CGCSR->GINTSTS.reg = 0xbfffffff;

	USB_OTG_FS_CGCSR->GINTMSK.b.RXFLVLM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.WUIM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.USBSUSPM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.USBRST = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.ENUMDNEM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.IEPINT = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.OEPINT = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.SOFM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.IISOIXFRM = 1;
	USB_OTG_FS_CGCSR->GINTMSK.b.IPXFRM = 1;
//	USB_OTG_FS_CGCSR->GINTMSK.b.SRQIM = 1;
//	USB_OTG_FS_CGCSR->GINTMSK.b.OTGINT = 1;
	

	/* Enable global int */
	ahbcfg.reg = 0;
	ahbcfg.b.GINTMSK = 1;
	USB_OTG_FS_CGCSR->GAHBCFG = ahbcfg;

	printf("Core init...done\n");

	return 0;
}

typedef uint32_t usb_ret_t;

struct usb_device;

struct usb_device_fops
{
	usb_ret_t (*sof)(struct usb_device * usbd);
	usb_ret_t (*get_device_descriptor)(struct usb_device * usbd, uint8_t ** buff, uint32_t * len);
};

struct usb_device
{
	struct
	{
		USB_OTG_FS_CGCSR_T * CGCSR;
		USB_OTG_FS_DMCSR_T * DMCSR;
		USB_OTG_FS_DIEPCTLx_T * DIEPCTL[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		USB_OTG_FS_DIEPINTx_T * DIEPINT[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		USB_OTG_FS_DIEPSIZx_T * DIEPSIZ[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		USB_OTG_FS_DOEPCTLx_T * DOEPCTL[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		USB_OTG_FS_DOEPINTx_T * DOEPINT[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		USB_OTG_FS_DOEPSIZx_T * DOEPSIZ[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		uint32_t * DFIFO[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
	} regs;
	struct usb_device_fops fops;
	struct usb_setup_packet setup_packet;
};

struct usb_device usbd;

usb_ret_t usb_cdc_sof(struct usb_device * usbd)
{

}

usb_ret_t usb_cdc_get_device_descriptor(struct usb_device * usbd, uint8_t ** buff, uint32_t * len)
{
#define USB_CDC_MAX_PACKET_SIZE0	64	
#define USB_CDC_VID			0x0483
#define USB_CDC_PID			0x5740
#define USB_CDC_STR_ID_MANUFACTURER	1
#define USB_CDC_STR_ID_PRODUCT		2
#define USB_CDC_STR_ID_SERIAL		3
#define USB_CDC_NUM_CFG			1
	/*
	 * {0x12, 0x1, 0x0, 0x2, 0x0, 0x0, 0x0, 0x40, 0x83, 0x4, 0x40, 0x57, 0x0, 0x2, 0x1, 0x2, 0x3, 0x1}
	 */
	static usb_device_descriptor_t desc = 
	{
		.bLength 		= sizeof(usb_device_descriptor_t),
		.bDescriptorType 	= 0x1,
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

	if(NULL != buff && NULL != len)
	{
		*buff = (uint8_t*)&desc;
		*len = sizeof(desc);
	}

	return 0;
}

int usb_init(void)
{
	int ret=0;
	memset(&usbd, 0, sizeof(usbd));
	ret = usb_init_io();
	if(ret)
	{
		err("init io\n");
		return -1;
	}

	ret = usb_init_core();
	if(ret)
	{
		err("init core\n");
		return -1;
	}

	ret = usb_init_irq();
	if(ret)
	{
		err("init irq\n");
		return -1;
	}
	usbd.regs.CGCSR = USB_OTG_FS_CGCSR;
	usbd.regs.DMCSR = USB_OTG_FS_DMCSR;
	for(int i=0;i<USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		usbd.regs.DFIFO[i] = USB_OTG_FS_DFIFO(i);
		usbd.regs.DIEPCTL[i] = USB_OTG_FS_DIEPCTL(i);
		usbd.regs.DIEPINT[i] = USB_OTG_FS_DIEPINT(i);
		usbd.regs.DIEPSIZ[i] = USB_OTG_FS_DIEPSIZ(i);
		usbd.regs.DOEPCTL[i] = USB_OTG_FS_DOEPCTL(i);
		usbd.regs.DOEPINT[i] = USB_OTG_FS_DOEPINT(i);
		usbd.regs.DOEPSIZ[i] = USB_OTG_FS_DOEPSIZ(i);
	}

	usbd.fops.sof = usb_cdc_sof;
	usbd.fops.get_device_descriptor = usb_cdc_get_device_descriptor;
}

typedef enum
{
	USB_MODE_DEVICE,
	USB_MODE_HOST,

	USB_MODE_MAX
} usb_mode_t;

usb_mode_t usb_get_mode(struct usb_device * usbd)
{
	return usbd->regs.CGCSR->GINTSTS.b.CMOD ? USB_MODE_HOST : USB_MODE_DEVICE;
}

USB_OTG_FS_GINTSTS_T usb_get_core_interrupts(struct usb_device * usbd)
{
	USB_OTG_FS_GINTSTS_T gintsts;

	gintsts = usbd->regs.CGCSR->GINTSTS;
	gintsts.reg &= usbd->regs.CGCSR->GINTMSK.reg;

	return gintsts;
}

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

void usb_dbg_print_gintsts(USB_OTG_FS_GINTSTS_T GINTSTS)
{
	PRINT_FIELD(GINTSTS, CMOD);
	PRINT_FIELD(GINTSTS, MMIS);
	PRINT_FIELD(GINTSTS, OTGINT);
	PRINT_FIELD(GINTSTS, SOF);
	PRINT_FIELD(GINTSTS, RXFLVL);
	PRINT_FIELD(GINTSTS, NPTXFE);
	PRINT_FIELD(GINTSTS, GINAKEFF);
	PRINT_FIELD(GINTSTS, GONAKEFF);
	PRINT_FIELD(GINTSTS, ESUSP);
	PRINT_FIELD(GINTSTS, USBSUSP);
	PRINT_FIELD(GINTSTS, USBRST);
	PRINT_FIELD(GINTSTS, ENUMDNE);
	PRINT_FIELD(GINTSTS, ISOODRP);
	PRINT_FIELD(GINTSTS, EOPF);
	PRINT_FIELD(GINTSTS, IEPINT);
	PRINT_FIELD(GINTSTS, OEPINT);
	PRINT_FIELD(GINTSTS, IISOIXFR);
	PRINT_FIELD(GINTSTS, IPXFR);
	PRINT_FIELD(GINTSTS, HPRTINT);
	PRINT_FIELD(GINTSTS, HCINT);
	PRINT_FIELD(GINTSTS, PTXFE);
	PRINT_FIELD(GINTSTS, CIDSCHG);
	PRINT_FIELD(GINTSTS, DISCINT);
	PRINT_FIELD(GINTSTS, SRQINT);
	PRINT_FIELD(GINTSTS, WKUPINT);
}

int usb_read_packet(struct usb_device * usbd, void * ptr, uint32_t len)
{
	uint32_t i=0;
	uint32_t * p = (uint32_t*)ptr;
	uint32_t len32 = (len+3)/4;
	
	__IO uint32_t * fifo = usbd->regs.DFIFO[0];

	for(i=0;i<len32;i++)
	{
		*(p++) = *fifo;
	}
	
	return len;
}

int usb_dev_flush_tx_fifo(struct usb_device * usbd, uint32_t n)
{
#define USB_DEV_FLUSH_TX_FIFO_DELAY_US		2000
	USB_OTG_FS_GRSTCTL_T grstctl;
	grstctl.reg = 0;
	grstctl.b.TXFFLSH = 1;
	grstctl.b.TXFNUM = n;
	usbd->regs.CGCSR->GRSTCTL = grstctl;
	int counter = 0;
	do
	{
		if(counter++ > USB_DEV_FLUSH_TX_FIFO_DELAY_US)
		{
			return -1;
		}
		delay_us(1);
	} while(usbd->regs.CGCSR->GRSTCTL.b.TXFFLSH == 1);
	
	return 0;
}

uint32_t usb_dev_irq_rxflvl(struct usb_device * usbd)
{
	/* Disable the Rx fifo non-empty interrupt */
	usbd->regs.CGCSR->GINTMSK.b.RXFLVLM = 0;

	USB_OTG_FS_GRXSTSx_T grxstsp = usbd->regs.CGCSR->GRXSTSP;
	USB_GRXSTSP_PKTSTS_T pktsts = grxstsp.b.PKTSTS;
	//dbg("PKTSTS=%d, BCNT=%d\n", pktsts, grxstsp.b.BCNT); 
	switch(pktsts)
	{
		case USB_GRXSTSP_PKSTS_GLOBAL_OUT_NAK:
			break;
		case USB_GRXSTSP_PKSTS_OUT_PACKET_REVEIVED:
			break;
		case USB_GRXSTSP_PKSTS_OUT_TX_COMPLETED:
			break;
		case USB_GRXSTSP_PKSTS_SETUP_COMPLETED:
			break;
		case USB_GRXSTSP_PKSTS_SETUP_PACKET_RECEIVED:
			usb_read_packet(usbd, &usbd->setup_packet, sizeof(usbd->setup_packet));
			//usb_dbg_print_setup_packet(&usbd->setup_packet);
			break;
		default:
			break;
	}

	/* Enable the Rx fifo non-empty interrupt */
	usbd->regs.CGCSR->GINTMSK.b.RXFLVLM = 1;

}

uint32_t usb_dev_irq_sof(struct usb_device * usbd)
{
	if(usbd->fops.sof)
	{
		usbd->fops.sof(usbd);
	}
	
	usbd->regs.CGCSR->GINTSTS.b.SOF = 1;

	return 0;
}

uint32_t usb_dev_read_all_out_ep_irq(struct usb_device * usbd)
{
	uint32_t oepint = 0;
	uint32_t oepm = 0;

	oepint = usbd->regs.DMCSR->DAINT.b.OEPINT;
	oepm = usbd->regs.DMCSR->DAINTMSK.b.OEPM;

	return (oepint & oepm);
}

uint32_t usb_dev_read_out_ep_irq(struct usb_device * usbd, uint32_t n)
{
	uint32_t doepint = 0;
	uint32_t doepmsk = 0;
	
	if(n < USB_OTG_FS_DEVICE_MODE_FIFO_NUM)
	{
		doepint = usbd->regs.DOEPINT[n]->reg;
		doepmsk = usbd->regs.DMCSR->DOEPMSK.reg;
	}

	return (doepint & doepmsk);
}

int usb_dev_ctl_send_data(struct usb_device * usbd, uint8_t * buff, uint32_t len)
{

}

int usb_dev_request_get_descriptor(struct usb_device * usbd, struct usb_setup_packet * setup_packet)
{
	uint8_t * buff = NULL;
	uint32_t len = 0;
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
	if(NULL != buff && len>0)
	{
		printf("{");
		for(uint32_t i=0;i<len-1;i++)
		{
			printf("0x%x, ", buff[i]);
		}
		printf("0x%x}\n", buff[len-1]);
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
		case USB_REQUEST_SET_ADDRESS:
			return -1;
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

int usb_dev_setup_done_irq(struct usb_device * usbd)
{
	switch(usbd->setup_packet.bmRequestType.b.Recipient)
	{
		case USB_REQUEST_RECIPIENT_DEVICE:
			return usb_dev_request_device(usbd, &usbd->setup_packet);
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

uint32_t usb_dev_out_ep_irq_process(struct usb_device * usbd, uint32_t n, USB_OTG_FS_DOEPINTx_T doepint)
{
	if(doepint.b.STUP)
	{
		usb_dev_setup_done_irq(usbd);
		usbd->regs.DOEPINT[n]->b.STUP = 1;
	}
}

uint32_t usb_dev_irq_usbrst(struct usb_device * usbd)
{
	/* Clear the Remote Wake-up signaling*/
	usbd->regs.DMCSR->DCTL.b.RWUSIG = 0;
	
	usb_dev_flush_tx_fifo(usbd, 0);

	for(int i = 0 ; i < USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		usbd->regs.DIEPINT[i]->reg = 0xFF;
		usbd->regs.DOEPINT[i]->reg = 0xFF;
	}

	usbd->regs.DMCSR->DAINTMSK.reg = 0xFFFFFFFF;
	usbd->regs.DMCSR->DAINTMSK.b.IEPM = 1;
	usbd->regs.DMCSR->DAINTMSK.b.OEPM = 1;
	
	USB_OTG_FS_DOEPMSK_T doepmsk;
	
	doepmsk.reg = 0;
	doepmsk.b.XFRCM = 1;
	doepmsk.b.EPDM = 1;
	doepmsk.b.STUPM = 1;

	usbd->regs.DMCSR->DOEPMSK = doepmsk;
	
	USB_OTG_FS_DIEPMSK_T diepmsk;

	diepmsk.reg = 0;
	diepmsk.b.XFRCM = 1;
	diepmsk.b.TOM = 1;
	diepmsk.b.EPDM = 1;

	usbd->regs.DMCSR->DIEPMSK = diepmsk;
	
	/* Reset device address*/
	usbd->regs.DMCSR->DCFG.b.DAD = 0;

#define USB_DEV_DOEPSIZ0_XFRXIZ		(8*3)
#define USB_DEV_DOEPSIZ0_PKTCNT		1
#define USB_DEV_DOEPSIZ0_STUPCNT	3

	USB_OTG_FS_DOEPSIZ0_T doepsiz0;
	doepsiz0.reg = 0;
	doepsiz0.b.XFRSIZ = USB_DEV_DOEPSIZ0_XFRXIZ;
	doepsiz0.b.PKTCNT = USB_DEV_DOEPSIZ0_PKTCNT;
	doepsiz0.b.STUPCNT = USB_DEV_DOEPSIZ0_STUPCNT;
	usbd->regs.DOEPSIZ[0]->reg = doepsiz0.reg;

	usbd->regs.CGCSR->GINTSTS.b.USBRST = 1;

	return 0;
}

uint32_t usb_dev_irq_oepint(struct usb_device * usbd)
{
	uint32_t ep_int = 0;

	ep_int = usb_dev_read_all_out_ep_irq(usbd);
	
	/* Process all endpoint interrupts */
	uint32_t epnum = 0;
	while(ep_int)
	{
		if(ep_int&0x1)
		{
			USB_OTG_FS_DOEPINTx_T doepint;
		        doepint.reg = usb_dev_read_out_ep_irq(usbd, epnum);
			if(doepint.reg)
			{
				usb_dev_out_ep_irq_process(usbd, epnum, doepint);
			}
		}
		epnum++;
		ep_int >>= 1;
	}

	return 0;
}

uint32_t usb_irq(struct usb_device * usbd)
{
	if(USB_MODE_DEVICE == usb_get_mode(usbd))
	{
		USB_OTG_FS_GINTSTS_T gintsts = usb_get_core_interrupts(usbd);
		//dbg("GINTSTS	: 0x%08x\n", gintsts.reg);
		
		if(!gintsts.reg)
		{
			dbg("Spurious interrupt\n");
			return 0;
		}
		
		if(0)
		{
			USB_OTG_FS_GINTSTS_T tmp = gintsts;
			tmp.b.SOF = 0;
			if(tmp.reg)
			{
				usb_dbg_print_gintsts(gintsts);
			}
		}

		if(gintsts.b.SOF)
		{
			usb_dev_irq_sof(usbd);
		}

		if(gintsts.b.RXFLVL)
		{
			//dbg("RXFLVL\n");
			usb_dev_irq_rxflvl(usbd);
		}
		
		if(gintsts.b.USBRST)
		{
			//dbg("USBRST\n");
			usb_dev_irq_usbrst(usbd);
		}

		if(gintsts.b.OEPINT)
		{
			usb_dev_irq_oepint(usbd);
		}
	}
	else
	{
		//err("USB in host mode\n");
	}
}

void OTG_FS_IRQHandler(void)
{
	usb_irq(&usbd);
}

void init(void)
{
	delay_init();
//	cec_init();
	usb_init();
}

int main(void)
{
	init();
	printf("HDMI Snoop\n");
	memset(&cec_message_buffer, 0, sizeof(cec_message_buffer));
	int i=0;
	for(;;)
	{
#if 0
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

