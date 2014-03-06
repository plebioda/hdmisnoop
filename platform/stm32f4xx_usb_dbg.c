#include <stm32f4xx_usb_dbg.h>

void usb_dev_platform_print_regs()
{
	__IO uint32_t * USB_OTG_DFIFO[] = {
		USB_OTG_FS_DFIFO(0),
		USB_OTG_FS_DFIFO(1),
		USB_OTG_FS_DFIFO(2),
		USB_OTG_FS_DFIFO(3),
	};
	USB_OTG_FS_DIEPCTLx_T * USB_OTG_DIEPCTL[] = {
		USB_OTG_FS_DIEPCTL(0),
		USB_OTG_FS_DIEPCTL(1),
		USB_OTG_FS_DIEPCTL(2),
		USB_OTG_FS_DIEPCTL(3),
	};
	USB_OTG_FS_DIEPINTx_T * USB_OTG_DIEPINT[] = {
		USB_OTG_FS_DIEPINT(0),
		USB_OTG_FS_DIEPINT(1),
		USB_OTG_FS_DIEPINT(2),
		USB_OTG_FS_DIEPINT(3),
	};
	USB_OTG_FS_DIEPSIZx_T * USB_OTG_DIEPSIZ[] = {
		USB_OTG_FS_DIEPSIZ(0),
		USB_OTG_FS_DIEPSIZ(1),
		USB_OTG_FS_DIEPSIZ(2),
		USB_OTG_FS_DIEPSIZ(3),
	};
	USB_OTG_FS_DTXFSTSx_T * USB_OTG_DTXFSTS[] = {
		USB_OTG_FS_DTXFSTS(0),
		USB_OTG_FS_DTXFSTS(1),
		USB_OTG_FS_DTXFSTS(2),
		USB_OTG_FS_DTXFSTS(3),
	};
	USB_OTG_FS_DOEPCTLx_T * USB_OTG_DOEPCTL[] = {
		USB_OTG_FS_DOEPCTL(0),
		USB_OTG_FS_DOEPCTL(1),
		USB_OTG_FS_DOEPCTL(2),
		USB_OTG_FS_DOEPCTL(3),

	};
	__IO USB_OTG_FS_DOEPINTx_T * USB_OTG_DOEPINT[] = {
		USB_OTG_FS_DOEPINT(0),
		USB_OTG_FS_DOEPINT(1),
		USB_OTG_FS_DOEPINT(2),
		USB_OTG_FS_DOEPINT(3),
	};
	__IO USB_OTG_FS_DOEPSIZx_T * USB_OTG_DOEPSIZ[] = {
		USB_OTG_FS_DOEPSIZ(0),
		USB_OTG_FS_DOEPSIZ(1),
		USB_OTG_FS_DOEPSIZ(2),
		USB_OTG_FS_DOEPSIZ(3),
	};

	dbg("*** USB OTG FS Registers ***\n");
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GOTGCTL);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GOTGINT);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GAHBCFG);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GUSBCFG);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GRSTCTL);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GINTSTS);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GINTMSK);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GRXSTSP);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GRXFSIZ);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXF0);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, HNPTXSTS);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, GCCFG);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, CID);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, HPTXFSIZ);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[0]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[1]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[2]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[3]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[4]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[5]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[6]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[7]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[8]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[9]);
	PRINT_REG_FIELD(USB_OTG_FS_, CGCSR, DIEPTXFx[10]);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DCFG);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DCTL);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DSTS);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DIEPMSK);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DOEPMSK);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DAINT);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DAINTMSK);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DVBUSDIS);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DVBUSPULS);
	PRINT_REG_FIELD(USB_OTG_FS_, DMCSR, DIEPEMPMSK);
	PRINT_REG(USB_OTG_FS_, PCGCCTL);
	PRINT_REG(USB_OTG_, DIEPCTL[0]);
	PRINT_REG(USB_OTG_, DIEPCTL[1]);
	PRINT_REG(USB_OTG_, DIEPCTL[2]);
	PRINT_REG(USB_OTG_, DIEPCTL[3]);
	PRINT_REG(USB_OTG_, DIEPINT[0]);
	PRINT_REG(USB_OTG_, DIEPINT[1]);
	PRINT_REG(USB_OTG_, DIEPINT[2]);
	PRINT_REG(USB_OTG_, DIEPINT[3]);
	PRINT_REG(USB_OTG_, DIEPSIZ[0]);
	PRINT_REG(USB_OTG_, DIEPSIZ[1]);
	PRINT_REG(USB_OTG_, DIEPSIZ[2]);
	PRINT_REG(USB_OTG_, DIEPSIZ[3]);
	PRINT_REG(USB_OTG_, DTXFSTS[0]);
	PRINT_REG(USB_OTG_, DTXFSTS[1]);
	PRINT_REG(USB_OTG_, DTXFSTS[2]);
	PRINT_REG(USB_OTG_, DTXFSTS[3]);
	PRINT_REG(USB_OTG_, DOEPCTL[0]);
	PRINT_REG(USB_OTG_, DOEPCTL[1]);
	PRINT_REG(USB_OTG_, DOEPCTL[2]);
	PRINT_REG(USB_OTG_, DOEPCTL[3]);
	PRINT_REG(USB_OTG_, DOEPINT[0]);
	PRINT_REG(USB_OTG_, DOEPINT[1]);
	PRINT_REG(USB_OTG_, DOEPINT[2]);
	PRINT_REG(USB_OTG_, DOEPINT[3]);
	PRINT_REG(USB_OTG_, DOEPSIZ[0]);
	PRINT_REG(USB_OTG_, DOEPSIZ[1]);
	PRINT_REG(USB_OTG_, DOEPSIZ[2]);
	PRINT_REG(USB_OTG_, DOEPSIZ[3]);
	PRINT_REG(USB_OTG_, DFIFO[0]);
	PRINT_REG(USB_OTG_, DFIFO[1]);
	PRINT_REG(USB_OTG_, DFIFO[2]);
	dbg("****************************\n");
}

