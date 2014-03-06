#ifndef _STM32F4XX_USB_DBG_H
#define _STM32F4XX_USB_DBG_H

#include <stm32f4xx_usb_fs.h>
#include <debug.h>

#ifndef DEBUG_CONFIG_USB_DEV_WRITE_PACKET
#define DEBUG_CONFIG_USB_DEV_WRITE_PACKET 	1
#endif

void usb_dev_platform_print_regs();
void usb_dev_platform_dbg_print_gintsts(USB_OTG_FS_GINTSTS_T GINTSTS);


#endif //_STM32F4XX_USB_DBG_H
