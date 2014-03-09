#include <usb.h>
#include <debug.h>
#include <usb_dbg.h>
#include <usb_dev_platform.h>
#include <stm32f4xx_usb_fs.h>
#include <stm32f4xx_usb_dbg.h>

struct usb_dev_platform
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
		USB_OTG_FS_DTXFSTSx_T * DTXFSTS[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
		__IO uint32_t * DFIFO[USB_OTG_FS_DEVICE_MODE_FIFO_NUM];
	} regs;
	struct usb_dev_platform_callbacks * callbacks;
	struct usb_setup_packet setup_packet;
} usb_dev_platform;

void usb_dbg_print_gintsts(USB_OTG_FS_GINTSTS_T GINTSTS);

#define USB_DEV_PLATFORM_CALLBACK(n, cb, args...)	if((n)->callbacks->cb)						\
							{								\
								(n)->callbacks->cb((n)->callbacks->context, ## args);	\
							}								\
							else								\
							{								\
								dbg("'%s' callback not initialized\n", STR(cb));	\
							}


int usb_dev_platform_write_packet(struct usb_dev_platform * usbd, uint32_t n, uint8_t * buff, uint16_t len)
{
	__IO uint32_t * fifo = (uint32_t*)usbd->regs.DFIFO[n];
	uint32_t * buff32 = (uint32_t*)buff;
#if DEBUG_CONFIG_USB_DEV_WRITE_PACKET
		dbg("%d: {", n);
		for(uint32_t i=0;i<len-1;i++)
		{
			_dbg("0x%x, ", buff[i]);
		}
		_dbg("0x%x}\n", buff[len-1]);
#endif
	uint32_t len32 = (len + 3) / 4;
	for(uint32_t i = 0 ; i < len32 ; i++)
	{
		*fifo = *(buff32++);
	}

	return len;
}

int usb_dev_platform_ep_stall(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep)
{
	ep->stall = 1;
	
	if(USB_ENDPOINT_DIR_IN == ep->dir)
	{
		USB_OTG_FS_DIEPCTLx_T diepctl;
		
		diepctl.reg = usbd->regs.DIEPCTL[ep->id]->reg;
		
		if(diepctl.b.EPENA)
		{
			diepctl.b.EPDIS = 1;
		}
		
		diepctl.b.STALL = 1;
		
		usbd->regs.DIEPCTL[ep->id]->reg = diepctl.reg;

		return 0;
	}
	else if(USB_ENDPOINT_DIR_OUT == ep->dir)
	{
		USB_OTG_FS_DOEPCTLx_T doepctl;

		doepctl.reg = usbd->regs.DOEPCTL[ep->id]->reg;

		doepctl.b.STALL = 1;

		usbd->regs.DOEPCTL[ep->id]->reg = doepctl.reg;

		return 0;
	}
	else
	{
		return -1;
	}
}

#define USB_DEV_DOEPSIZ0_XFRSIZ		(8*3)
#define USB_DEV_DOEPSIZ0_PKTCNT		1
#define USB_DEV_DOEPSIZ0_STUPCNT	3

int usb_dev_platform_rx_setup(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep)
{
	if(ep->id == 0)
	{
		USB_OTG_FS_DOEPSIZ0_T doepsiz;

		doepsiz.reg = 0;
		doepsiz.b.STUPCNT = USB_DEV_DOEPSIZ0_STUPCNT;
		doepsiz.b.PKTCNT = USB_DEV_DOEPSIZ0_PKTCNT;
		doepsiz.b.XFRSIZ = USB_DEV_DOEPSIZ0_XFRSIZ;

		usbd->regs.DOEPSIZ[0]->reg = doepsiz.reg;

		return 0;
	}
	else
	{
		TODO("Implement me");
		return -1;
	}
}

int usb_dev_platform_rx_prepare(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep)
{
	if(ep->id == 0)
	{
		USB_OTG_FS_DOEPCTL0_T doepctl;
		USB_OTG_FS_DOEPSIZ0_T doepsiz;

		doepctl.reg = usbd->regs.DOEPCTL[0]->reg;
		doepsiz.reg = usbd->regs.DOEPSIZ[0]->reg;

		doepsiz.b.PKTCNT = 1;
		doepsiz.b.XFRSIZ = ep->mps;

		usbd->regs.DOEPSIZ[0]->reg = doepsiz.reg = 0;

		doepctl.b.CNAK = 1;
		doepctl.b.EPENA = 1;

		usbd->regs.DOEPCTL[0]->reg = doepctl.reg;

		return 0;
	}
	else
	{
		TODO("Implement me");
		return -1;
	}
}

int usb_dev_platform_write_fifo(usb_dev_platform_handle_t usbd, struct usb_device_endpoint * ep)
{
	uint16_t len = ep->buff.len;
	
	uint32_t n = ep->id;

	uint16_t len32 = (len + 3) / 4;

	if(ep->buff.len > 0 && usbd->regs.DTXFSTS[n]->b.INEPTFSAV > len32)
	{
		usb_dev_platform_write_packet(usbd, n, ep->buff.ptr, ep->buff.len);
		ep->buff.len = 0;
	}

	return 0;
}

uint32_t usb_dev_platform_in_ep_irq_process(struct usb_dev_platform * usbd, uint32_t n, USB_OTG_FS_DIEPINTx_T diepint)
{
	if(diepint.b.XFRC)
	{
		usbd->regs.DMCSR->DIEPEMPMSK.b.INEPTXFEM |= (1 << n);
		
		USB_DEV_PLATFORM_CALLBACK(usbd, tx_completed, n);
		
		usbd->regs.DIEPINT[n]->b.XFRC = 1;
	}
	if(diepint.b.EPDISD)
	{
		usbd->regs.DIEPINT[n]->b.EPDISD = 1;
	}
	if(diepint.b.ITTXFE)
	{
		usbd->regs.DIEPINT[n]->b.ITTXFE = 1;
	}
	if(diepint.b.INEPNE)
	{
		usbd->regs.DIEPINT[n]->b.INEPNE = 1;
	}
	if(diepint.b.TOC)
	{
		usbd->regs.DIEPINT[n]->b.TOC = 1;
	}
	if(diepint.b.TXFE)
	{
		USB_DEV_PLATFORM_CALLBACK(usbd, tx_fifo_empty, n);
		usbd->regs.DIEPINT[n]->b.TXFE = 1;
	}

	return 0;
}

int usb_init_irq(void)
{
	NVIC_SetPriority(OTG_FS_IRQn, 2);
	NVIC_EnableIRQ(OTG_FS_IRQn);

	return 0;
}

uint8_t usb_dev_platform_ep_type_to_reg(usb_endpoint_type_t type)
{
	switch(type)
	{
		case USB_ENDPOINT_TYPE_CONTROL:
			return USB_DEPCTL_EPTYP_CONTROL;
		case USB_ENDPOINT_TYPE_ISOCHRONOUS:
			return USB_DEPCTL_EPTYP_ISOCHRONOUS;
		case USB_ENDPOINT_TYPE_BULK:
			return USB_DEPCTL_EPTYP_BULK;
		case USB_ENDPOINT_TYPE_INTERRUPT:
			return USB_DEPCTL_EPTYP_INTERRUPT;
		default:
			return USB_DEPCTL_EPTYP_CONTROL;
	}
}

usb_mode_t usb_dev_platform_get_mode(struct usb_dev_platform * usbd)
{
	return usbd->regs.CGCSR->GINTSTS.b.CMOD ? USB_MODE_HOST : USB_MODE_DEVICE;
}

USB_OTG_FS_GINTSTS_T usb_get_core_interrupts(struct usb_dev_platform * usbd)
{
	USB_OTG_FS_GINTSTS_T gintsts;

	gintsts = usbd->regs.CGCSR->GINTSTS;
	gintsts.reg &= usbd->regs.CGCSR->GINTMSK.reg;

	return gintsts;
}

int usb_read_packet(struct usb_dev_platform * usbd, void * ptr, uint32_t len)
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

int usb_dev_platform_flush_tx_fifo(struct usb_dev_platform * usbd, uint32_t n)
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
	
	delay_us(3);

	return 0;
}

uint32_t usb_dev_platform_irq_rxflvl(struct usb_dev_platform * usbd)
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
			break;
		default:
			break;
	}

	/* Enable the Rx fifo non-empty interrupt */
	usbd->regs.CGCSR->GINTMSK.b.RXFLVLM = 1;
}

uint32_t usb_dev_platform_irq_sof(struct usb_dev_platform * usbd)
{
	USB_DEV_PLATFORM_CALLBACK(usbd, sof);

	usbd->regs.CGCSR->GINTSTS.b.SOF = 1;

	return 0;
}

uint32_t usb_dev_platform_read_all_in_ep_irq(struct usb_dev_platform * usbd)
{
	uint32_t iepint = 0;
	uint32_t iepm = 0;

	iepint = usbd->regs.DMCSR->DAINT.b.IEPINT;
	iepm = usbd->regs.DMCSR->DAINTMSK.b.IEPM;

	return (iepint & iepm);
}

uint32_t usb_dev_platform_read_all_out_ep_irq(struct usb_dev_platform * usbd)
{
	uint32_t oepint = 0;
	uint32_t oepm = 0;

	oepint = usbd->regs.DMCSR->DAINT.b.OEPINT;
	oepm = usbd->regs.DMCSR->DAINTMSK.b.OEPM;

	return (oepint & oepm);
}

uint32_t usb_dev_platform_read_in_ep_irq(struct usb_dev_platform * usbd, uint32_t n)
{
	uint32_t diepint = 0;
	uint32_t diepmsk = 0;
	
	if(n < USB_OTG_FS_DEVICE_MODE_FIFO_NUM)
	{
		diepmsk = usbd->regs.DMCSR->DIEPMSK.reg;
		uint32_t empmsk = usbd->regs.DMCSR->DIEPEMPMSK.reg;

		diepmsk |= ((empmsk >> n) & 0x1) << 7;

		diepint = usbd->regs.DIEPINT[n]->reg;
	}

	return (diepint & diepmsk);

}

uint32_t usb_dev_platform_read_out_ep_irq(struct usb_dev_platform * usbd, uint32_t n)
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

int usb_dev_platform_ep0_tx_start(struct usb_dev_platform * usbd, struct usb_device_endpoint * ep)
{
	USB_OTG_FS_DIEPSIZ0_T diepsiz;
	
	diepsiz.reg = 0;
	
	if(!ep->buff.len)
	{
		diepsiz.b.XFRSIZ = 0;
		diepsiz.b.PKTCNT = 1;
	}
	else
	{
		diepsiz.b.XFRSIZ = ep->buff.len;
		diepsiz.b.PKTCNT = 1;
	}
	
	usbd->regs.DIEPSIZ[ep->id]->reg = diepsiz.reg;

	USB_OTG_FS_DIEPCTL0_T diepctl;
	
	diepctl.reg = usbd->regs.DIEPCTL[ep->id]->reg;
	
	diepctl.b.CNAK = 1;
	diepctl.b.EPENA = 1;

	usbd->regs.DIEPCTL[ep->id]->reg = diepctl.reg;
	dbg("tx_start: DIEPCTL[%d] = 0x%08x\n", ep->id, usbd->regs.DIEPCTL[ep->id]->reg);
	
	usbd->regs.DMCSR->DIEPEMPMSK.b.INEPTXFEM |= (1 << ep->id);

	return 0;
}

uint32_t usb_dev_platform_out_ep_irq_process(struct usb_dev_platform * usbd, uint32_t n, USB_OTG_FS_DOEPINTx_T doepint)
{
	if(doepint.b.STUP)
	{
		USB_DEV_PLATFORM_CALLBACK(usbd, setup_done, &usbd->setup_packet);
		
		usbd->regs.DOEPINT[n]->b.STUP = 1;
	}

	return 0;
}

int usb_dev_platform_ep_activate(struct usb_dev_platform * usbd, struct usb_device_endpoint * ep)
{
	if(USB_ENDPOINT_DIR_IN == ep->dir)
	{
		USB_OTG_FS_DIEPCTLx_T diepctl;
		
		diepctl.reg = usbd->regs.DIEPCTL[ep->id]->reg;

		if(!diepctl.b.USBAEP)
		{
			diepctl.b.MPSIZ = ep->mps;
			diepctl.b.EPTYP = usb_dev_platform_ep_type_to_reg(ep->type);
			diepctl.b.TXFNUM = ep->id;
			diepctl.b.SD0PID = 1;
			diepctl.b.USBAEP = 1;

			usbd->regs.DIEPCTL[ep->id]->reg = diepctl.reg;
		}

		usbd->regs.DMCSR->DAINTMSK.b.IEPM |= (1 << ep->id);
	}
	else
	{
		USB_OTG_FS_DOEPCTLx_T doepctl;
		
		doepctl.reg = usbd->regs.DOEPCTL[ep->id]->reg;

		if(!doepctl.b.USBAEP)
		{
			doepctl.b.MPSIZ = ep->mps;
			doepctl.b.EPTYP = usb_dev_platform_ep_type_to_reg(ep->type);
			doepctl.b.SD0PID = 1;
			doepctl.b.USBAEP = 1;

			usbd->regs.DOEPCTL[ep->id]->reg = doepctl.reg;
		}

		usbd->regs.DMCSR->DAINTMSK.b.OEPM |= (1 << ep->id);
	}

	return 0;
}

uint32_t usb_dev_platform_irq_usbrst(struct usb_dev_platform * usbd)
{
	/* Clear the Remote Wake-up signaling*/
	usbd->regs.DMCSR->DCTL.b.RWUSIG = 0;
	
	usb_dev_platform_flush_tx_fifo(usbd, 0);

	for(int i = 0 ; i < USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		usbd->regs.DIEPINT[i]->reg = 0xFF;
		usbd->regs.DOEPINT[i]->reg = 0xFF;
	}

	usbd->regs.DMCSR->DAINT.reg = 0xFFFFFFFF;
	
	USB_OTG_FS_DAINTMSK_T daintmsk;

	daintmsk.reg = 0;
	daintmsk.b.IEPM = 1;
	daintmsk.b.OEPM = 1;

	usbd->regs.DMCSR->DAINTMSK = daintmsk;
	
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

	USB_OTG_FS_DOEPSIZ0_T doepsiz0;
	doepsiz0.reg = 0;
	doepsiz0.b.XFRSIZ = USB_DEV_DOEPSIZ0_XFRSIZ;
	doepsiz0.b.PKTCNT = USB_DEV_DOEPSIZ0_PKTCNT;
	doepsiz0.b.STUPCNT = USB_DEV_DOEPSIZ0_STUPCNT;
	usbd->regs.DOEPSIZ[0]->reg = doepsiz0.reg;

	USB_OTG_FS_GINTSTS_T gintsts;
	gintsts.reg = 0;
	gintsts.b.USBRST = 1;
	usbd->regs.CGCSR->GINTSTS = gintsts;
	
	USB_DEV_PLATFORM_CALLBACK(usbd, reset);

	return 0;
}

uint32_t usb_dev_platform_irq_iepint(struct usb_dev_platform * usbd)
{
	uint32_t ep_int = 0;

	ep_int = usb_dev_platform_read_all_in_ep_irq(usbd);

	/* Process all out endpoint interrupts */
	uint32_t epnum = 0;
	while(ep_int)
	{
		if(ep_int&0x1)
		{
			USB_OTG_FS_DIEPINTx_T diepint;
			diepint.reg = usb_dev_platform_read_in_ep_irq(usbd, epnum);
			if(diepint.reg)
			{
				usb_dev_platform_in_ep_irq_process(usbd, epnum, diepint);
			}
		}
		epnum++;
		ep_int >>= 1;
	}

}

uint32_t usb_dev_platform_irq_oepint(struct usb_dev_platform * usbd)
{
	uint32_t ep_int = 0;

	ep_int = usb_dev_platform_read_all_out_ep_irq(usbd);
	
	/* Process all out endpoint interrupts */
	uint32_t epnum = 0;
	while(ep_int)
	{
		if(ep_int&0x1)
		{
			USB_OTG_FS_DOEPINTx_T doepint;
		        doepint.reg = usb_dev_platform_read_out_ep_irq(usbd, epnum);
			if(doepint.reg)
			{
				usb_dev_platform_out_ep_irq_process(usbd, epnum, doepint);
			}
		}
		epnum++;
		ep_int >>= 1;
	}

	return 0;
}

usb_speed_t usb_dev_platform_get_speed(struct usb_dev_platform * usbd)
{
	/*TODO*/
	return USB_SPEED_FULL;
}

int usb_dev_platform_ep0_activate(struct usb_dev_platform * usbd)
{
	USB_OTG_FS_DSTS_T dsts = usbd->regs.DMCSR->DSTS;
	USB_OTG_FS_DIEPCTL0_T diepctl;
	
	diepctl.reg = usbd->regs.DIEPCTL[0]->reg;
	diepctl.b.MPSIZ = USB_DIEPCTL_MPSIZ_64;
	usbd->regs.DIEPCTL[0]->reg = diepctl.reg;
	
	usbd->regs.DMCSR->DCTL.b.CGINAK = 1;

	return 0;
}

int usb_dev_platform_irq_enumdne(struct usb_dev_platform * usbd)
{
	int ret = 0;
	
	usb_dev_platform_ep0_activate(usbd);
	
	usb_speed_t speed = usb_dev_platform_get_speed(usbd);

	if(USB_SPEED_FULL == speed)
	{
		USB_DEV_PLATFORM_CALLBACK(usbd, enum_done, speed);
		
		usbd->regs.CGCSR->GUSBCFG.b.TRDT = 5;
	}
	else
	{
		dbg("Wrong speed value\n");
		ret = -1;
	}
	
	USB_OTG_FS_GINTSTS_T gintsts;
	gintsts.reg = 0;
	gintsts.b.ENUMDNE = 1;
	usbd->regs.CGCSR->GINTSTS = gintsts;

	return ret;
}

int usb_dev_platform_set_address(usb_dev_platform_handle_t handle, usb_address_t addr)
{
	USB_OTG_FS_DCFG_T dcfg = handle->regs.DMCSR->DCFG;
	
	dcfg.b.DAD = addr;
	
	handle->regs.DMCSR->DCFG = dcfg;

	return 0;
}

uint32_t usb_irq(struct usb_dev_platform * usbd)
{
	static int _iepint = 0;
	if(USB_MODE_DEVICE == usb_dev_platform_get_mode(usbd))
	{
		USB_OTG_FS_GINTSTS_T gintsts = usb_get_core_interrupts(usbd);
		
		if(!gintsts.reg)
		{
			dbg("Spurious interrupt\n");
			return 0;
		}
		
		if(gintsts.b.OEPINT)
		{
			dbg("OEPINT\n");
			usb_dev_platform_irq_oepint(usbd);
		}

		if(gintsts.b.IEPINT)
		{
			//dbg("IEPINT\n");
			usb_dev_platform_irq_iepint(usbd);
		}

		if(gintsts.b.SOF)
		{
			usb_dev_platform_irq_sof(usbd);
		}

		if(gintsts.b.RXFLVL)
		{
			dbg("RXFLVL\n");
			usb_dev_platform_irq_rxflvl(usbd);
		}
		
		if(gintsts.b.ENUMDNE)
		{
			dbg("ENUMDNE\n");
			usb_dev_platform_irq_enumdne(usbd);
		}

		if(gintsts.b.USBRST)
		{
			dbg("USBRST\n");
			usb_dev_platform_irq_usbrst(usbd);
		}
	}
}

void OTG_FS_IRQHandler(void)
{
	usb_irq(&usb_dev_platform);
}

int usb_init_io()
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
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8_1|GPIO_OSPEEDER_OSPEEDR8_0);

	GPIOA->AFR[1] &= ~0xf;
	GPIOA->AFR[1] |= 0xa;

	/*
	 *  PA9 - USB OTG FS VBUS
	 */
	/* Input */
	GPIOA->MODER &=~(GPIO_MODER_MODER9);
	/* Open drain */
	//GPIOA->OTYPER |= (GPIO_OTYPER_OT_9);
	/* No pull-up, no pull-down */
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR9);
	/* Speed */
	//GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR9);
	//GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR9_1);

	/*
	 *  PA10 - USB OTG FS ID
	 */
	/* Input */
	GPIOA->MODER &=~(GPIO_MODER_MODER10);
	/* Open drain */
	//GPIOA->OTYPER |= (GPIO_OTYPER_OT_10);
	/* Pull-up*/
	GPIOA->PUPDR &=~(GPIO_PUPDR_PUPDR10);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR10_0);
	/* Speed */
	//GPIOA->OSPEEDR &=~(GPIO_OSPEEDER_OSPEEDR10);
	//GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR10_1);
	GPIOA->AFR[1] &= ~0xf00;
	GPIOA->AFR[1] |= 0xa00;

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
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR11_1|GPIO_OSPEEDER_OSPEEDR11_0);

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
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR12_1|GPIO_OSPEEDER_OSPEEDR12_0);

	GPIOA->AFR[1] &= ~0xf0000;
	GPIOA->AFR[1] |= 0xa0000;
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;

	return 0;
}

int usb_init_core()
{
	USB_OTG_FS_GAHBCFG_T ahbcfg;
	USB_OTG_FS_GUSBCFG_T usbcfg;
	USB_OTG_FS_GCCFG_T gccfg;

	/* Disable global Interrupt */
	USB_OTG_FS_CGCSR->GAHBCFG.b.GINTMSK = 0;
	
	/*
	 * Core Init
	 */
	dbg("Core init...\n");
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

	/*
	 * Wait for AHB master IDLE state
	 */
	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			dbg("AHBIDL delay\n");
			return -1;
		}
	} while(USB_OTG_FS_CGCSR->GRSTCTL.b.AHBIDL == 0);

	/*
	 * Core Soft Reset
	 */
	USB_OTG_FS_CGCSR->GRSTCTL.b.CSRST = 1;
	counter = 0;
	do
	{
		delay_us(100);
		if(counter++ > 20000) 
		{
			dbg("CSRST delay\n");
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
	gccfg.b.NOVBUSSENS = 1;
	USB_OTG_FS_CGCSR->GCCFG = gccfg;
	delay_us(20000);

	/*
	 * Force device mode
	 */
	usbcfg = USB_OTG_FS_CGCSR->GUSBCFG;
	usbcfg.b.FHMOD = 0;
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
	USB_OTG_FS_DMCSR->DCFG = dcfg;
	
	/* Full speed */
	dcfg = USB_OTG_FS_DMCSR->DCFG;
	dcfg.b.DSPD = 3;
	USB_OTG_FS_DMCSR->DCFG = dcfg;

	USB_OTG_FS_CGCSR->GRXFSIZ.b.RXFD = 128;

	/* EP0 TX */
	/*TODO check addr */
	USB_OTG_FS_DIEPTXF0_T dieptxf0;
	dieptxf0.b.TX0FD = 32;
	dieptxf0.b.TX0FSA = 128;
	dieptxf0.reg = 0x00200080;
	USB_OTG_FS_CGCSR->DIEPTXF0.reg = dieptxf0.reg;

	/* EP1 TX */
	USB_OTG_FS_DIEPTXFx_T dieptxf1;
	dieptxf1.b.INEPTXSA = dieptxf0.b.TX0FSA + dieptxf0.b.TX0FD; 
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
	dieptxf3.b.INEPTXFD = 0;
	USB_OTG_FS_CGCSR->DIEPTXFx[2] = dieptxf3;

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
			dbg("TX FIFO flush delay\n");
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
			dbg("RX FIFO flush delay\n");
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

	/*TODO what is this bit ???? */
	USB_OTG_FS_DMCSR->DIEPMSK.reg |= (1<<8);

	/*
	 * Enable Device Mode Interrupts
	 */
	USB_OTG_FS_GINTMSK_T intmsk;
	intmsk.reg = 0;
	USB_OTG_FS_CGCSR->GINTMSK = intmsk;
	
	/*
	 * Enable common interrupts
	 */
	USB_OTG_FS_CGCSR->GOTGINT.reg = 0xffffffff;

	USB_OTG_FS_CGCSR->GINTSTS.reg = 0xbfffffff;
	
	intmsk.b.WUIM = 1;
	intmsk.b.USBSUSPM = 1;
	intmsk.b.RXFLVLM = 1;
	intmsk.b.USBRST = 1;
	intmsk.b.ENUMDNEM = 1;
	intmsk.b.IEPINT = 1;
	intmsk.b.OEPINT = 1;
	intmsk.b.SOFM = 1;
	intmsk.b.IISOIXFRM = 1;
	intmsk.b.IPXFRM = 1;
	
	USB_OTG_FS_CGCSR->GINTMSK = intmsk;
	
	/* Enable global int */
	ahbcfg = USB_OTG_FS_CGCSR->GAHBCFG;
	ahbcfg.b.GINTMSK = 1;
	USB_OTG_FS_CGCSR->GAHBCFG = ahbcfg;

	dbg("Core init...done\n");

	return 0;
}

usb_dev_platform_handle_t usb_dev_platform_init(struct usb_dev_platform_callbacks * callbacks)
{
	int ret = usb_init_io();
	
	if(ret)
	{
		dbg("init io\n");
		return NULL;
	}

	ret = usb_init_core();
	
	if(ret)
	{
		dbg("init core\n");
		return NULL;
	}

	ret = usb_init_irq();
	if(ret)
	{
		dbg("init irq\n");
		return NULL;
	}
	
	usb_dev_platform_handle_t usbd = &usb_dev_platform;
	
	usbd->regs.CGCSR = USB_OTG_FS_CGCSR;
	usbd->regs.DMCSR = USB_OTG_FS_DMCSR;
	
	for(int i=0;i<USB_OTG_FS_DEVICE_MODE_FIFO_NUM; i++)
	{
		usbd->regs.DFIFO[i] = USB_OTG_FS_DFIFO(i);
		usbd->regs.DIEPCTL[i] = USB_OTG_FS_DIEPCTL(i);
		usbd->regs.DIEPINT[i] = USB_OTG_FS_DIEPINT(i);
		usbd->regs.DIEPSIZ[i] = USB_OTG_FS_DIEPSIZ(i);
		usbd->regs.DOEPCTL[i] = USB_OTG_FS_DOEPCTL(i);
		usbd->regs.DOEPINT[i] = USB_OTG_FS_DOEPINT(i);
		usbd->regs.DOEPSIZ[i] = USB_OTG_FS_DOEPSIZ(i);
		usbd->regs.DTXFSTS[i] = USB_OTG_FS_DTXFSTS(i);
	}

	usb_dev_platform.callbacks = callbacks;

	return usbd;
}

