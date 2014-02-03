#ifndef _USBD_TYPES_H
#define _USBD_TYPES_H

#include <stm32f4xx.h>

#define USB_OTG_FS_BASE			(AHB2PERIPH_BASE + 0x00000000)
#define USB_OTG_HS_BASE			(AHB1PERIPH_BASE + 0x00040000)
#define USB_OTG_FS_CGCSR_BASE		(USB_OTG_FS_BASE + 0x00000000)
#define USB_OTG_FS_HMCSR_BASE		(USB_OTG_FS_BASE + 0x00000400)
#define USB_OTG_FS_DMCSR_BASE		(USB_OTG_FS_BASE + 0x00000800)
#define USB_OTG_FS_DIEPCTL0_BASE	(USB_OTG_FS_BASE + 0x00000900)
#define USB_OTG_FS_DIEPCTLx_BASE(x)	(USB_OTG_FS_DIEPCTL0_BASE + (x)*0x20)
#define USB_OTG_FS_DIEPINT0_BASE	(USB_OTG_FS_BASE + 0x00000908)
#define USB_OTG_FS_DIEPINTx_BASE(x)	(USB_OTG_FS_DIEPINT0_BASE + (x)*0x20)
#define USB_OTG_FS_DIEPSIZ0_BASE	(USB_OTG_FS_BASE + 0x00000910)
#define USB_OTG_FS_DIEPSIZx_BASE(x)	(USB_OTG_FS_DIEPSIZ0_BASE + (x)*0x20)
#define USB_OTG_FS_DTXFSTS0_BASE	(USB_OTG_FS_BASE + 0x00000918)
#define USB_OTG_FS_DTXFSTSx_BASE(x)	(USB_OTG_FS_DTXFSTS0_BASE + (x)*0x20)
#define USB_OTG_FS_DOEPCTL0_BASE	(USB_OTG_FS_BASE + 0x00000B00)
#define USB_OTG_FS_DOEPCTLx_BASE(x)	(USB_OTG_FS_DOEPCTL0_BASE + (x)*0x20)
#define USB_OTG_FS_DOEPINT0_BASE	(USB_OTG_FS_BASE + 0x00000B08)
#define USB_OTG_FS_DOEPINTx_BASE(x)	(USB_OTG_FS_DOEPINT0_BASE + (x)*0x20)
#define USB_OTG_FS_DOEPSIZ0_BASE	(USB_OTG_FS_BASE + 0x00000B08)
#define USB_OTG_FS_DOEPSIZx_BASE(x)	(USB_OTG_FS_DOEPSIZ0_BASE + (x)*0x20)

#define USB_OTG_FS_CGCSR		((USB_OTG_FS_CGCSR_T*)USB_OTG_FS_CGCSR_BASE)
#define USB_OTG_FS_HMCSR		((USB_OTG_FS_HMCSR_T*)USB_OTG_FS_HMCSR_BASE)
#define USB_OTG_FS_DMCSR		((USB_OTG_FS_DMCSR_T*)USB_OTG_FS_DMCSR_BASE)
#define USB_OTG_FS_DIEPCTL0		((USB_OTG_FS_DIEPCTL0_T*)USB_OTG_FS_DIEPCTL0_BASE)
#define USB_OTG_FS_DIEPCTL(x)		((USB_OTG_FS_DIEPCTLx_T*)USB_OTG_FS_DIEPCTLx_BASE((x)))
#define USB_OTG_FS_DIEPINT0		((USB_OTG_FS_DIEPINTx_T*)USB_OTG_FS_DIEPINT0_BASE)
#define USB_OTG_FS_DIEPINT(x)		((USB_OTG_FS_DIEPINTx_T*)USB_OTG_FS_DIEPINTx_BASE((x)))
#define USB_OTG_FS_DIEPSIZ0		((USB_OTG_FS_DIEPSIZ0_T*)USB_OTG_FS_DIEPSIZ0_BASE)
#define USB_OTG_FS_DIEPSIZ(x)		((USB_OTG_FS_DIEPSIZx_T*)USB_OTG_FS_DIEPSIZx_BASE((x)))
#define USB_OTG_FS_DTXFSTS0		((USB_OTG_FS_DTXFSTSx_T*)USB_OTG_FS_DTXFSTS0_BASE)
#define USB_OTG_FS_DTXFSTS(x)		((USB_OTG_FS_DTXFSTSx_T*)USB_OTG_FS_DTXFSTSx_BASE((x)))
#define USB_OTG_FS_DOEPCTL0		((USB_OTG_FS_DOEPCTL0_T*)USB_OTG_FS_DOEPCTL0_BASE)
#define USB_OTG_FS_DOEPCTL(x)		((USB_OTG_FS_DOEPCTLx_T*)USB_OTG_FS_DOEPCTLx_BASE((x)))
#define USB_OTG_FS_DOEPINT0		((USB_OTG_FS_DOEPINTx_T*)USB_OTG_FS_DOEPINT0_BASE)
#define USB_OTG_FS_DOEPINT(x)		((USB_OTG_FS_DOEPINTx_T*)USB_OTG_FS_DOEPINTx_BASE((x)))
#define USB_OTG_FS_DOEPSIZ0		((USB_OTG_FS_DOEPSIZ0_T*)USB_OTG_FS_DOEPSIZ0_BASE)
#define USB_OTG_FS_DOEPSIZ(x)		((USB_OTG_FS_DOEPSIZx_T*)USB_OTG_FS_DOEPSIZx_BASE((x)))


typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t SRQSCS 	: 1;	/* Bit 0      : Session request success */
		uint32_t SRQ		: 1;	/* Bit 1      : Session request */
		uint32_t reserved_7_2	: 6;	/* Bits 7:2   : reserved */
		uint32_t HNGSCS		: 1;	/* Bit 8      : Host negotiation success */
		uint32_t HNPRQ		: 1;	/* Bit 9      : HNP request */
		uint32_t HSHNPEN	: 1; 	/* Bit 10     : host set HNP enable */
		uint32_t DHNPEN		: 1;	/* Bit 11     : device HDP enabled */
		uint32_t reserved_15_12 : 4;	/* Bits 15:12 : reserved */
		uint32_t CIDSTS		: 1;	/* Bit 16     : Connector ID status */
		uint32_t DBCT		: 1; 	/* Bit 17     : Long/short debounce time */
		uint32_t ASVLD		: 1;	/* Bit 18     : A-session valid */
		uint32_t BSVLD		: 1; 	/* Bit 19     : B-session valid */
		uint32_t reserved_31_20 :11;	/* Bits 31:20 : reserved */
	} b;
} USB_OTG_FS_GOTGCTL_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t reserved_1_0	: 2;	/* Bits 1:0   : reserved */
		uint32_t SEDET		: 1;	/* Bit 2      : Session end detected */
		uint32_t SRSSCHG	: 1; 	/* Bit 3      : Session request success status change */
		uint32_t reserved_7_3	: 5;	/* Bits 7:3   : reserved */
		uint32_t HNSSCHG	: 1; 	/* Bit 9      : host negotiation success status change */
		uint32_t reserved_16_10 : 7;	/* Bits 16:10 : reserved */
		uint32_t HNGDET		: 1; 	/* Bit 17     : Host negotiation detect */
		uint32_t ADTOCHG	: 1;	/* Bit 18     : A-device timeout change */
		uint32_t DBCDNE		: 1;	/* Bit 19     : Debounce done */
		uint32_t reserved_31_20 :11;	/* Bits 31:20 : reserved */	
	} b;
} USB_OTG_FS_GOTGINT_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t GINTMSK	: 1;	/* Bit 0      : Global interrupt mask */
		uint32_t reserved_6_1	: 6;	/* Bits 6:1   : reserved */
		uint32_t TXFELVL	: 1;	/* Bit 7      : Tx FIFO empty level */
		uint32_t PTXFELVL	: 1;	/* Bit 8      : Periodic Tx FIFO empty level */
		uint32_t reserved_31_9  :23;	/* Bits 31:9  : reserved */
	} b;
} USB_OTG_FS_GAHBCFG_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t TOCAL		: 3;	/* Bits 2:0   : FS timeout calibration */
		uint32_t reserved_5_3	: 3;	/* Bits 5:3   : reserved */
		uint32_t PHYSEL		: 1;	/* Bit 6      : Full Speed serial transceiver select */
		uint32_t reserved_7	: 1;	/* Bit 7      : reserved */
		uint32_t SRPCAP		: 1;	/* Bit 8      : SRP-capable */
		uint32_t HNPCAP		: 1;	/* Bit 9      : HNP-capable */
		uint32_t TRDT		: 4;	/* Bits 13:10 : USB turnaround time */
		uint32_t reserved_28_14 :15;	/* Bits 28:14 : reserved */
		uint32_t FHMOD		: 1;	/* Bit 29     : Force host mode */
		uint32_t FDMOD		: 1; 	/* Bit 30     : Force device mode */
		uint32_t CTXPKT		: 1;	/* Bit 31     : Corrupt Tx packet */
	} b;
} USB_OTG_FS_GUSBCFG_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t CSRST		: 1;	/* Bit 0      : Core soft reset */
		uint32_t HSRST		: 1;	/* Bit 1      : HCLK soft reset */
		uint32_t FCRST		: 1;	/* Bit 2      : Host frame counter reset */
		uint32_t reserved_3	: 1;	/* Bit 3      : reserved */
		uint32_t RXFFLSH	: 1;	/* Bit 4      : Rx FIFO flush */
		uint32_t TXFFLSH	: 1;	/* Bit 5      : Tx FIFO flush */
		uint32_t TXFNUM		: 5;	/* Bits 10:6  : Tx FIFO number */
		uint32_t reserved_30_11 :20;	/* Bits 30:11 : reserved */
		uint32_t AHBIDL		: 1;	/* Bit 31     : AHB master idle */	
	} b;
} USB_OTG_FS_GRSTCTL_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t CMOD		: 1;	/* Bit 0      : Current mode of operation */
		uint32_t MMIS		: 1;	/* Bit 1      : Mode mismatch interrupt */
		uint32_t OTGINT		: 1;	/* Bit 2      : OTG interrupt */
		uint32_t SOF		: 1;	/* Bit 3      : Start of frame */
		uint32_t RXFLVL		: 1;	/* Bit 4      : Rx FIFO non-empty */
		uint32_t NPTXFE		: 1;	/* Bit 5      : Non-periodic Tx FIFO empty */
		uint32_t GINAKEFF	: 1;	/* Bit 6      : Global IN non-periodic NAK effective */
		uint32_t GONAKEFF	: 1;	/* Bit 7      : Global OUT NAK effective */
		uint32_t reserved_9_8	: 2;	/* Bits 9:8   : reserved */
		uint32_t ESUSP		: 1;	/* Bit 10     : Early suspend */
		uint32_t USBSUSP	: 1;	/* Bit 11     : USB suspend */
		uint32_t USBRST		: 1;	/* Bit 12     : USB reset */
		uint32_t ENUMDNE	: 1;	/* Bit 13     : Enumeration done */
		uint32_t ISOODRP	: 1;	/* Bit 14     : Isochronous OUT packet dropped interrupt */
		uint32_t EOPF		: 1;	/* Bit 15     : End of periodic frame interrupt */
		uint32_t reserved_17_16 : 2;	/* Bits 17:16 : reserved */
		uint32_t IEPINT		: 1;	/* Bit 18     : IN endpoint interrupt */
		uint32_t OEPINT		: 1;	/* Bit 19     : OUT endpoint interrupt */
		uint32_t IISOIXFR	: 1;	/* Bit 20     : Incomplete isochronous IN transfer */
		uint32_t IPXFR		: 1;	/* Bit 21     : Incomplete periodic transfer */
		uint32_t reserved_23_22 : 2;	/* Bits 23:22 : reserved */
		uint32_t HPRTINT	: 1;	/* Bit 24     : Host port interrupt */
		uint32_t HCINT		: 1;	/* Bit 25     : Host channels interrupt */
		uint32_t PTXFE		: 1;	/* Bit 26     : Periodic Tx FIFO empty */
		uint32_t reserved_27	: 1;	/* Bit 27     : reserved */
		uint32_t CIDSCHG	: 1;	/* Bit 28     : Connector ID status change */
		uint32_t DISCINT	: 1;	/* Bit 29     : Disconnect detect interrupt */
		uint32_t SRQINT		: 1;	/* Bit 30     : Session request/new session detected interrupt */
		uint32_t WKUPINT	: 1;	/* Bit 31     : Resume/remote wakeup detected interrupt */
	} b;
} USB_OTG_FS_GINTSTS_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t reserved_0	: 1;	/* Bit 0      : reserved */
		uint32_t MMISM		: 1;	/* Bit 1      : Mode mismatch interrupt */
		uint32_t OTGINT		: 1;	/* Bit 2      : OTG interrupt */
		uint32_t SOFM		: 1;	/* Bit 3      : Start of frame */
		uint32_t RXFLVLM	: 1;	/* Bit 4      : Rx FIFO non-empty */
		uint32_t NPTXFEM	: 1;	/* Bit 5      : Non-periodic Tx FIFO empty */
		uint32_t GINAKEFFM	: 1;	/* Bit 6      : Global IN non-periodic NAK effective */
		uint32_t GONAKEFFM	: 1;	/* Bit 7      : Global OUT NAK effective */
		uint32_t reserved_9_8	: 2;	/* Bits 9:8   : reserved */
		uint32_t ESUSPM		: 1;	/* Bit 10     : Early suspend */
		uint32_t USBSUSPM	: 1;	/* Bit 11     : USB suspend */
		uint32_t USBRST		: 1;	/* Bit 12     : USB reset */
		uint32_t ENUMDNEM	: 1;	/* Bit 13     : Enumeration done */
		uint32_t ISOODRPM	: 1;	/* Bit 14     : Isochronous OUT packet dropped interrupt */
		uint32_t EOPFM		: 1;	/* Bit 15     : End of periodic frame interrupt */
		uint32_t reserved_16 	: 1;	/* Bit 16     : reserved */
		uint32_t EPMISM		: 1;	/* Bit 17     : Endpoint mismatch interrupt mask */
		uint32_t IEPINT		: 1;	/* Bit 18     : IN endpoint interrupt */
		uint32_t OEPINT		: 1;	/* Bit 19     : OUT endpoint interrupt */
		uint32_t IISOIXFRM	: 1;	/* Bit 20     : Incomplete isochronous IN transfer */
		uint32_t IPXFRM		: 1;	/* Bit 21     : Incomplete periodic transfer */
		uint32_t reserved_23_22 : 2;	/* Bits 23:22 : reserved */
		uint32_t HPRTIM		: 1;	/* Bit 24     : Host port interrupt */
		uint32_t HCIM		: 1;	/* Bit 25     : Host channels interrupt */
		uint32_t PTXFEM		: 1;	/* Bit 26     : Periodic Tx FIFO empty */
		uint32_t reserved_27	: 1;	/* Bit 27     : reserved */
		uint32_t CIDSCHGM	: 1;	/* Bit 28     : Connector ID status change */
		uint32_t DISCINT	: 1;	/* Bit 29     : Disconnect detect interrupt */
		uint32_t SRQIM		: 1;	/* Bit 30     : Session request/new session detected interrupt */
		uint32_t WUIM		: 1;	/* Bit 31     : Resume/remote wakeup detected interrupt */
	} b;
} USB_OTG_FS_GINTMSK_T;

typedef union
{
	uint32_t reg;
	union	
	{
	struct
	{
		/*TODO: Host mode*/
		uint32_t reg;
	} h;
	struct 
	{
		uint32_t EPNUM		: 4;	/* Bits 3:0   : Endpoint number*/
		uint32_t BCNT		:11;	/* Bits 14:4  : Byte count */
		uint32_t DPID		: 2;	/* Bits 16:15 : Data PID */
		uint32_t PKTSTS		: 4;	/* Bits 20:17 : Packet status */
		uint32_t FRMNUM		: 4;	/* Bits 24:21 : Frame number */
		uint32_t reserved_31_25 : 7;	/* Bits 31:25 : reserved */
	} d;
	} b;
} USB_OTG_FS_GRXSTSx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t RXFD		:16;	/* Bits 15:0  : Rx FIFO depth */
		uint16_t reserved_31_16 :16;	/* Bits 31:16 : reserved */
	} b;
} USB_OTG_FS_GRXFSIZ_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t NPTXFD		:16;	/* Bits 15:0  : Non-periodic Tx FIFO depth */
	       	uint16_t reserved_31_16 :16;	/* Bits 31:16 : reserved */
	} b;
} USB_OTG_FS_HNPTXFSIZ_T;

typedef union
{
	uint32_t reg;
	struct
	{
		/* TODO In device mode this register is not valid */
		uint32_t reg;
	} b;
} USB_OTG_FS_HNPTXSTS_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t reserved_15_0	:16;	/* Bits 15:0  : reserved */
		uint32_t PWRDWN		: 1;	/* Bit 16     : Power Down */
		uint32_t reserved_17	: 1;	/* Bit 17     : reserved */
		uint32_t VBUSASEN	: 1;	/* Bit 18     : Enable the VBus sensing A device */
		uint32_t VBUSBSEN	: 1;	/* Bit 19     : Enable the VBus sensing B device */
		uint32_t SOFOUTEN	: 1;	/* Bit 20     : SOF output enable */
		uint32_t NOVBUSSENS	: 1;	/* Bit 21     : VBus sensing disable option */
		uint32_t reserved_31_22 :10;	/* Bits 31:22 : reserved */
	} b;
} USB_OTG_FS_GCCFG_T;

typedef uint32_t USB_OTG_FS_CID_T;


typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t PTXSA		:16;	/* Bits 15:0  : Host periodic Tx FIFO depth */
	       	uint16_t reserved_31_16 :16;	/* Bits 31:16 : reserved */
	} b;
} USB_OTG_FS_HPTXFSIZ_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t INEPTXSA	:16;	/* Bits 15:0  : IN endpoint FIFOx transmit RAM start address */
	       	uint16_t INEPTXFD	:16;	/* Bits 31:16 : IN endpoint Tx FIFO depth */
	} b;
} USB_OTG_FS_DIEPTXFx_T;

typedef struct
{
	__IO USB_OTG_FS_GOTGCTL_T 	GOTGCTL;		/* 0x000 : OTG FS control and statur register */
	__IO USB_OTG_FS_GOTGINT_T 	GOTGINT; 		/* 0x004 : OTG FS interrupt register */
	__IO USB_OTG_FS_GAHBCFG_T 	GAHBCFG;		/* 0x008 : OTG FS AHB configuration register */
	__IO USB_OTG_FS_GUSBCFG_T 	GUSBCFG;		/* 0x00C : OTG FS USB configuration register */
	__IO USB_OTG_FS_GRSTCTL_T 	GRSTCTL;		/* 0x010 : OTG FS reset register */
	__IO USB_OTG_FS_GINTSTS_T 	GINTSTS;		/* 0x014 : OTG FS core interrupt register */
	__IO USB_OTG_FS_GINTMSK_T 	GINTMSK;		/* 0x018 : OTG FS interrupt mask register */
	__IO USB_OTG_FS_GRXSTSx_T 	GRXSTSR;		/* 0x01C : OTG FS status read register */
	__IO USB_OTG_FS_GRXSTSx_T 	GRXSTSP;		/* 0x020 : OTG FS status pop register */
	__IO USB_OTG_FS_GRXFSIZ_T 	GRXFSIZ;		/* 0x024 : OTG FS receive FIFO size register */
	__IO USB_OTG_FS_HNPTXFSIZ_T	HNPTXFSIZ;		/* 0x028 : */
#define DIEPTXF0  HNPTXFSIZ	
	__IO USB_OTG_FS_HNPTXSTS_T 	HNPTXSTS;		/* 0x02C : */
	__IO uint32_t 			reserved_0x030;		/* 0x030 : */
	__IO uint32_t 			reserved_0x034;		/* 0x034 : */
	__IO USB_OTG_FS_GCCFG_T 	GCCFG;			/* 0x038 : OTG FS general core configuration register */
	__IO USB_OTG_FS_CID_T 		CID;			/* 0x03C : OTG FS core ID register */
	__IO uint32_t 			reserved_0x040[48];	/* 0x040 : */
	__IO USB_OTG_FS_HPTXFSIZ_T 	HPTXFSIZ;		/* 0x100 : OTG FS host periodic transmit FIFO size register */
	__IO USB_OTG_FS_DIEPTXFx_T 	DIEPTXFx[11];		/* 0x104 : */
} USB_OTG_FS_CGCSR_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t DSPD		: 2;	/* Bits 1:0   : Device speed */
		uint32_t NZLSOHSK	: 1;	/* Bit 2      : Non-zero-length status OUT handshake */
		uint32_t reserved_3	: 1;	/* Bit 3      : reserved */
		uint32_t DAD		: 7;	/* Bits 10:4  : Device address */
		uint32_t PFIVL		: 2;	/* Bits 12:11 : Periodic frame interval */
		uint32_t reserved_31_13 :19;	/* Bits 31:13 : reserved */
	} b;
} USB_OTG_FS_DCFG_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t RWUSIG		: 1;	/* Bit 0      : Remote wakeup signalling */
		uint32_t SDIS		: 1;	/* Bit 1      : Soft disconnect */
		uint32_t GINSTS		: 1;	/* Bit 2      : Global IN NAK status */
		uint32_t GONSTS		: 1;	/* Bit 3      : Global OUT NAK status */
		uint32_t TCTL		: 3;	/* Bits 6:4   : Test control */
		uint32_t SGINAK		: 1;	/* Bit 7      : Set global IN NAK */
		uint32_t CGINAK		: 1;	/* Bit 8      : Clear global IN NAK */
		uint32_t SGONAK		: 1;	/* Bit 9      : Set global OUT NAK */
		uint32_t CGONAK		: 1;	/* Bit 10     : Clear global OUT NAK */
		uint32_t POPRGDNE	: 1;	/* Bit 11     : Power-on programming done */
		uint32_t reserved_31_12 :20;	/* Bits 31:12 : reserved */
	} b;
} USB_OTG_FS_DCTL_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t SUSPSTS	: 1;	/* Bit 0      : Suspend status */
		uint32_t ENUMSPD	: 2;	/* Bits 2:1   : Enumerated speed */
		uint32_t EERR		: 1;	/* Bit 3      : Erratic error */
		uint32_t reserved_7_4	: 4;	/* Bits 7:4   : reserved */
		uint32_t FNSOF		:14;	/* Bits 21:8  : Frame numer of received SOF */
		uint32_t reserved_31_22 :10;	/* Bits 31:22 : reserved */
	} b;
} USB_OTG_FS_DSTS_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRCM		: 1;	/* Bit 0      : Transfer completed interrupt mask */
		uint32_t EPDM		: 1;	/* Bit 1      : Endpoint disabled interrupt mask */
		uint32_t reserved_2	: 1;	/* Bit 2      : reserved */
		uint32_t TOM		: 1;	/* Bit 3      : Timeout condition mask */
		uint32_t ITTXFEMSK	: 1;	/* Bit 4      : IN token received when Tx FIFO empty mask */
		uint32_t INEPNMM	: 1;	/* Bit 5      : IN token received with EP mismatch mask */
		uint32_t INEPNEM	: 1;	/* Bit 6      : IN endpoint NAK effective maskt */
		uint32_t reserved_31_7	:25;	/* Bits 31:7  : reserved */
	} b;
} USB_OTG_FS_DIEPMSK_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRCM		: 1;	/* Bit 0      : Transfer completed interrupt mask */
		uint32_t EPDM		: 1;	/* Bit 1      : Endpoint disabled interrupt mask */
		uint32_t reserved_2	: 1;	/* Bit 2      : reserved */
		uint32_t STUPM		: 1;	/* Bit 3      : SETUP phase done mask */
		uint32_t OTEPDM		: 1;	/* Bit 4      : OUT token received when endpoint disabled mask */
		uint32_t reserved_31_5	:27;	/* Bits 31:5  : reserved */
	} b;
} USB_OTG_FS_DOEPMSK_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t IEPINT		:16;	/* Bits 15:0  : IN endpoint interrupt bits */
		uint16_t OEPINT		:16;	/* Bits 31:16 : OUT endpoint interrupt bits */ 
	} b;
} USB_OTG_FS_DAINT_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t IEPM		:16;	/* Bits 15:0  : IN endpoint interrupt mask bits */
		uint16_t OEPM		:16;	/* Bits 31:16 : OUT endpoint interrupt mask bits */ 
	} b;
} USB_OTG_FS_DAINTMSK_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t VBUSDT		:16;	/* Bits 15:0  : VBUS discharge time */
		uint16_t reserved_31_16	:16;	/* Bits 31:16 : reserved */
	} b;
} USB_OTG_FS_DVBUSDIS_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t DVBUSP		:12;	/* Bits 11:0  : VBUS pulsing time */
		uint32_t reserved_31_12	:20;	/* Bits 31:12 : reserved */
	} b;
} USB_OTG_FS_DVBUSPULS_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t INEPTXFEM	:16;	/* Bits 15:0  : IN endpoint Tx FIFO empty interrupt mask bits */
		uint16_t reserved_31_16	:16;	/* Bits 31:16 : reserved */
	} b;
} USB_OTG_FS_DIEPEMPMSK_T;

typedef struct
{
	__IO USB_OTG_FS_DCFG_T 		DCFG;			/* 0x800 : OTG FS device configuration register */
	__IO USB_OTG_FS_DCTL_T 		DCTL;			/* 0x804 : OTG FS device control register */
	__IO USB_OTG_FS_DSTS_T 		DSTS;			/* 0x808 : OTG FS device status register */
	__IO uint32_t 			reserved_0x80C;		/* 0x80C : reserved */
	__IO USB_OTG_FS_DIEPMSK_T 	DIEPMSK;		/* 0x810 : OTG FS device IN endpoint common interrupt mask register */
	__IO USB_OTG_FS_DOEPMSK_T 	DOEPMSK;		/* 0x814 : OTG FS device OUT endpoint common interrupt mask register */
	__IO USB_OTG_FS_DAINT_T 	DAINT;			/* 0x818 : OTG FS device all endpoints interrupt register */
	__IO USB_OTG_FS_DAINTMSK_T 	DAINTMSK;		/* 0x81C : OTG FS device all endpoints interrupt mask register */
	__IO uint32_t 			reserved_0x820;		/* 0x820 : reserved */
	__IO uint32_t 			reserved_0x824;		/* 0x824 : reserved */
	__IO USB_OTG_FS_DVBUSDIS_T	DVBUSDIS;		/* 0x828 : OTG FS device VBUS discharge time register */
	__IO USB_OTG_FS_DVBUSPULS_T	DVBUSPULS;		/* 0x82C : OTG FS device VBUS pulsing time register */
	__IO uint32_t 			reserved_0x830;		/* 0x830 : reserved */
	__IO USB_OTG_FS_DIEPEMPMSK_T	DIEPEMPMSK;		/* 0x834 : OTG FS device IN endpoint FIFO empty interrupt mask register */
} USB_OTG_FS_DMCSR_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t MPSIZ		: 2;	/* Bits 1:0    : Maximum packet size */
		uint32_t reserved_14_2	:13;	/* Bits 14:2   : reserved */
		uint32_t USBAEP		: 1;	/* Bit 15      : USB active endpoint */
		uint32_t reserved_16	: 1;	/* Bit 16      : reserved */
		uint32_t NAKSTS		: 1;	/* Bit 17      : NAK status */
		uint32_t EPTYP		: 2;	/* Bits 19:18  : Endpoint type */
		uint32_t reserved_20	: 1;	/* Bit 20      : reserved */
		uint32_t STALL		: 1;	/* Bit 21      : STALL handshake */
		uint32_t TXFNUM		: 4;	/* Bits 25:22  : Tx FIFO number */
		uint32_t CNAK		: 1;	/* Bit 26      : Clear NAK */
		uint32_t SNAK		: 1;	/* Bit 27      : Set NAK */
		uint32_t reserved_29_28 : 2;	/* Bits 29:28  : reserved */
		uint32_t EPDIS		: 1;	/* Bit 30      : Endpoint disable */
		uint32_t EPENA		: 1;	/* Bit 31      : Endpoint enable */
	} b;
} USB_OTG_FS_DIEPCTL0_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t MPSIZ		:11;	/* Bits 10:0   : Maximum packet size */
		uint32_t reserved_14_11	: 4;	/* Bits 14:2   : reserved */
		uint32_t USBAEP		: 1;	/* Bit 15      : USB active endpoint */
		uint32_t DPID		: 1;	/* Bit 16      : Endpoint data PID [for interrupt/bulk]*/
						/*               Even/Odd frame number [for isochronous]*/
		uint32_t NAKSTS		: 1;	/* Bit 17      : NAK status */
		uint32_t EPTYP		: 2;	/* Bits 19:18  : Endpoint type */
		uint32_t reserved_20	: 1;	/* Bit 20      : reserved */
		uint32_t STALL		: 1;	/* Bit 21      : STALL handshake */
		uint32_t TXFNUM		: 4;	/* Bits 25:22  : Tx FIFO number */
		uint32_t CNAK		: 1;	/* Bit 26      : Clear NAK */
		uint32_t SNAK		: 1;	/* Bit 27      : Set NAK */
		uint32_t SD0PID		: 1;	/* Bit 28      : Set DATA0 PID [for interrupt/bulk] */
						/*               Set even frame [for isochronous] */
		uint32_t SD1PID		: 1;	/* Bit 29      : Set DATA1 PID [for interrupt/bulk] */
						/*               Set odd frame [for isochronous] */
		uint32_t EPDIS		: 1;	/* Bit 30      : Endpoint disable */
		uint32_t EPENA		: 1;	/* Bit 31      : Endpoint enable */
	} b;
} USB_OTG_FS_DIEPCTLx_T;

#define EONUM	DPID
#define SEVNFRM	SD0PID
#define SODDFRM	SD1PID

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t MPSIZ		: 2;	/* Bits 1:0    : Maximum packet size */
		uint32_t reserved_14_2	:13;	/* Bits 14:2   : reserved */
		uint32_t USBAEP		: 1;	/* Bit 15      : USB active endpoint */
		uint32_t reserved_16	: 1;	/* Bit 16      : reserved */
		uint32_t NAKSTS		: 1;	/* Bit 17      : NAK status */
		uint32_t EPTYP		: 2;	/* Bits 19:18  : Endpoint type */
		uint32_t SNPM		: 1;	/* Bit 20      : Snoop mode */
		uint32_t STALL		: 1;	/* Bit 21      : STALL handshake */
		uint32_t reserved_25_22	: 4;	/* Bits 25:22  : reserved */
		uint32_t CNAK		: 1;	/* Bit 26      : Clear NAK */
		uint32_t SNAK		: 1;	/* Bit 27      : Set NAK */
		uint32_t reserved_29_28 : 2;	/* Bits 29:28  : reserved */
		uint32_t EPDIS		: 1;	/* Bit 30      : Endpoint disable */
		uint32_t EPENA		: 1;	/* Bit 31      : Endpoint enable */
	} b;
} USB_OTG_FS_DOEPCTL0_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t MPSIZ		:11;	/* Bits 10:0   : Maximum packet size */
		uint32_t reserved_14_11	: 4;	/* Bits 14:2   : reserved */
		uint32_t USBAEP		: 1;	/* Bit 15      : USB active endpoint */
		uint32_t DPID		: 1;	/* Bit 16      : Endpoint data PID [for interrupt/bulk]*/
						/*               Even/Odd frame number [for isochronous]*/
		uint32_t NAKSTS		: 1;	/* Bit 17      : NAK status */
		uint32_t EPTYP		: 2;	/* Bits 19:18  : Endpoint type */
		uint32_t SNMP		: 1;	/* Bit 20      : Snoop mode */
		uint32_t STALL		: 1;	/* Bit 21      : STALL handshake */
		uint32_t reserved_25_22	: 4;	/* Bits 25:22  : Tx FIFO number */
		uint32_t CNAK		: 1;	/* Bit 26      : Clear NAK */
		uint32_t SNAK		: 1;	/* Bit 27      : Set NAK */
		uint32_t SD0PID		: 1;	/* Bit 28      : Set DATA0 PID [for interrupt/bulk] */
						/*               Set even frame [for isochronous] */
		uint32_t SD1PID		: 1;	/* Bit 29      : Set DATA1 PID [for interrupt/bulk] */
						/*               Set odd frame [for isochronous] */
		uint32_t EPDIS		: 1;	/* Bit 30      : Endpoint disable */
		uint32_t EPENA		: 1;	/* Bit 31      : Endpoint enable */
	} b;
} USB_OTG_FS_DOEPCTLx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRC		: 1;	/* Bit 0       : Transfer completed interrupt */
		uint32_t EPDISD		: 1;	/* Bit 1       : Endpoint disabled interrupt */
		uint32_t reserved_2	: 1;	/* Bit 2       : reserved */
		uint32_t TOC		: 1;	/* Bit 3       : Timout condition */
		uint32_t ITTXFE		: 1;	/* Bit 4       : IN token received when Tx FIFO is empty */
		uint32_t reserved_5 	: 1;	/* Bit 5       : reserved */
		uint32_t INEPNE		: 1;	/* Bit 6       : IN endpoint NAK effective */
		uint32_t TXFE		: 1;	/* Bit 7       : Transmit FIFO empty */
		uint32_t reserved_31_8	:24;	/* Bits 31:8   : reserved */
	} b;
} USB_OTG_FS_DIEPINTx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRC		: 1;	/* Bit 0       : Transfer completed interrupt */
		uint32_t EPDISD		: 1;	/* Bit 1       : Endpoint disabled interrupt */
		uint32_t reserved_2	: 1;	/* Bit 2       : reserved */
		uint32_t STUP		: 1;	/* Bit 3       : SETUP phase done */
		uint32_t OTEPDIS	: 1;	/* Bit 4       : OUT token received when endpoint disabled */
		uint32_t reserved_5 	: 1;	/* Bit 5       : reserved */
		uint32_t B2BSTUP	: 1;	/* Bit 6       : Back-to-Back SETUP packets received */
		uint32_t reserved_31_7	:25;	/* Bits 31:7   : reserved */
	} b;
} USB_OTG_FS_DOEPINTx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRSIZ		: 7;	/* Bits 6:0    : Transfer size */
		uint32_t reserved_18_7	:12;	/* Bits 18:7   : reserved */
		uint32_t PKTCNT		: 2;	/* Bits 20:19  : Packet count */
		uint32_t reserved_31_21 :11;	/* Bits 31:21  : reserved */
	} b;
} USB_OTG_FS_DIEPSIZ0_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRSIZ		: 7;	/* Bits 6:0    : Transfer size */
		uint32_t reserved_18_7	:12;	/* Bits 18:7   : reserved */
		uint32_t PKTCNT		: 1;	/* Bit 19      : Packet count */
		uint32_t reserved_28_20 : 9;	/* Bits 28:20  : reserved */
		uint32_t STUPCNT	: 2;	/* Bits 30:29  : SETUP packet count */
		uint32_t reserved_31	: 1;	/* Bit 31      : reserved */
	} b;
} USB_OTG_FS_DOEPSIZ0_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRSIZ		:19;	/* Bits 18:0   : Transfer size */
		uint32_t PKTCNT		:10;	/* Bits 28:19  : Packet count */
		uint32_t MCNT		: 2;	/* Bits 30:29  : Multi count */
		uint32_t reserved_31	: 1;	/* Bits 31     : reserved */
	} b;
} USB_OTG_FS_DIEPSIZx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t XFRSIZ		:19;	/* Bits 18:0   : Transfer size */
		uint32_t PKTCNT		:10;	/* Bits 28:19  : Packet count */
		uint32_t RXDPID		: 2;	/* Bits 30:29  : Received data PID [isochronous]*/
						/*               SETUP packet count [control]*/
		uint32_t reserved_31	: 1;	/* Bits 31     : reserved */
	} b;
} USB_OTG_FS_DOEPSIZx_T;

#define STUPCNT	RXDPID

typedef union
{
	uint32_t reg;
	struct
	{
		uint16_t INEPTFSAV	:16;	/* Bits 15:0   : IN endpoint Tx FIFO space available */
		uint32_t reserved_31_16 :16;	/* Bits 31:16  : reserved */
	} b;
} USB_OTG_FS_DTXFSTSx_T;

typedef union
{
	uint32_t reg;
	struct
	{
		uint32_t STPPCLK	: 1;	/* Bit 0       : Stop PHY clock */
		uint32_t GATEHCLK	: 1;	/* Bit 1       : Gate HCLK */
		uint32_t reserved_3_2	: 2;	/* Bits 3:2    : reserved */
		uint32_t PHYSUSP	: 1;	/* Bit 4       : PHY suspended */
		uint32_t reserved_31_5	:27;	/* Bits 31:5   : reserved */
	} b;
} USB_OTG_FS_PCGCCTL_T;

#endif //_USBD_TYPES_H
