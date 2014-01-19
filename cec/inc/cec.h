#ifndef _CEC_H
#define _CEC_H

#include <stdint.h>
#include <stm32f4xx.h>

/*
 * CEC line GPIO port
 */ 
#define CEC_GPIO	GPIOD

/*
 * CEC line pin number
 */ 
#define CEC_PIN		0

/*
 * CEC line GPIO Clock Enable Register
 */
#define CEC_PIN_ENR		AHB1ENR

/*
 * CEC line GPIO Clocke Enable Register field
 */
#define CEC_PIN_ENR_GPIO	RCC_AHB1ENR_GPIODEN

/*
 * CEC External Interrupt
 */
#define CEC_EXTI	EXTI0_IRQn

/*
 * CEC External Interrupt Configuration Register number
 */
#define CEC_EXTICR	0

/*
 * CEC External Interrupt Line
 */
#define CEC_EXTI_LINE	0

/*
 * CEC External Interrupt source input 
 * 0x3 = PD[x] pin
 */
#define CEC_EXTI_PIN	0x3

/*
 * CEC External Interrupt Mask Register
 */
#define CEC_EXTI_IMR	EXTI_IMR_MR0

/*
 * CEC External Interrupt Rising trigger selection register
 */
#define CEC_EXTI_RTSR	EXTI_RTSR_TR0

/*
 * CEC External Interrupt Falling trigger selection registre
 */
#define CEC_EXTI_FTSR	EXTI_FTSR_TR0

/*
 * CEC External Interrupt Pending Register
 */
#define CEC_EXTI_PR	EXTI_PR_PR0

/*
 * CEC External Interrupt Priority
 */
#define CEC_EXTI_PRIORITY	2

/*
 * Timings in us
 */ 
#define CEC_TIME_UNIT	100

/*
 * The start bit has to be validated by its low duration (a) and its total duration (b)
 *
 *                                  b
 *       |<------------------------------------------------->|
 *                        a
 *       |<-------------------------->|
 * ______                       .......______________________.......
 *       |                      :     |     :          :     |     :
 *       |                      :     |     :          :     |     :
 *       |______________________:_____|.....:          :.....|_____:____
 *       :                      :     :     :          :     :     :
 *       0ms                 3.5ms 3.7ms 3.9ms      4.3ms  4.5ms  4.7ms
 */
#define CEC_START_BIT_MIN_L	3500
#define CEC_START_BIT_NOM_L	3700
#define CEC_START_BIT_MAX_L	3900
#define CEC_START_BIT_NOM_H	8000
#define CEC_START_BIT_MIN_T	4300
#define CEC_START_BIT_NOM_T	4500
#define CEC_START_BIT_MAX_T	4700

#define CEC_DATAx_BIT_MIN_T	2050
#define CEC_DATAx_BIT_NOM_T	2400
#define CEC_DATAx_BIT_MAX_T	2750
#define CEC_DATAx_BIT_NOM_S	1050

#define CEC_DATA0_BIT_MIN_L	1300
#define CEC_DATA0_BIT_NOM_L	1500
#define CEC_DATA0_BIT_MAX_L	1700
#define CEC_DATA0_BIT_NOM_H	900

#define CEC_DATA1_BIT_MIN_L	400
#define CEC_DATA1_BIT_NOM_L	600
#define CEC_DATA1_BIT_MAX_L	800
#define CEC_DATA1_BIT_NOM_H	1800

#define CEC_DURATION(name)	((CEC_##name)/CEC_TIME_UNIT)

#define CEC_MESSAGE_MAX_LENGTH		16
#define CEC_MESSAGE_MAX_OPERAND_LENGTH	(CEC_MESSAGE_MAX_LENGTH-1)

#define CEC_ACK		1
#define CEC_NACK	0

struct cec_message_header
{
	uint8_t initiator : 4;
	uint8_t follower  : 4;
};

struct cec_rx_message
{
	struct cec_message_header header;
	uint8_t length;
	uint8_t ack;
	union
	{
		struct
		{
			uint8_t opcode;
			uint8_t operand[CEC_MESSAGE_MAX_OPERAND_LENGTH];
		} data;
		uint8_t buff[CEC_MESSAGE_MAX_LENGTH];
	} message;
};

#define CEC_RX_FILTER_ALL_DEVICES	0xffff
#define CEC_RX_FILTER_NO_DEVICES	0x0000

union logical_address
{
	uint16_t all;
	struct
	{
		uint16_t TV			: 1;
		uint16_t Recording_Device_1	: 1;
		uint16_t Recording_Device_2	: 1;
		uint16_t STB1			: 1;
		uint16_t DVD1			: 1;
		uint16_t Audio			: 1;
		uint16_t STB2			: 1;
		uint16_t STB3			: 1;
		uint16_t DVD2			: 1;
		uint16_t Recording_Device_3	: 1;
		uint16_t Reserved_10		: 1;
		uint16_t Reserved_11		: 1;
		uint16_t Reserved_12		: 1;
		uint16_t Reserved_13		: 1;
		uint16_t FreeUse		: 1;
		uint16_t Broadcast		: 1;
	} device;
};

struct cec_rx_filter
{
	union logical_address receive;
	struct
	{
		union logical_address follower;
		union logical_address initiator;
	} monitor;
};

typedef enum
{
	CEC_RESULT_OK = 0,
	CEC_ERROR_PARAM = 1,
	CEC_ERROR_RX_DROPPED = 2,
	CEC_ERROR_RX_START_BIT = 3,
	CEC_ERROR_RX_HEADER_BLOCK = 4,
	CEC_ERROR_RX_DATA_BLOCK = 5,
	CEC_ERROR_RX_MONITOR_NACK = 6,
	CEC_ERROR_RX_MONITOR_DATA_BLOCK = 7
} cec_result_t;

void cec_time_tick(void);
void cec_init(void);
cec_result_t cec_rx_message(struct cec_rx_message * message, struct cec_rx_filter * rx_filter);

#endif //_CEC_H
