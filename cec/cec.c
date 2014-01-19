/**
 * @file cec.c
 * @author Pawel Lebioda <pawel.lebioda89@gmail.com>
 * @brief This file contains definitions of all CEC functions
 */ 

#include <cec.h>
#include <stdio.h>
#include <clock.h>
/**
 * @addtogroup CEC
 * @{
 */

#define CEC_RX_DATA_BIT_ERROR		0x80
#define CEC_RX_DATA_BIT_ERROR_T		0x40
#define CEC_RX_DATA_BIT_ERROR_L0	0x20
#define CEC_RX_DATA_BIT_ERROR_L1	0x10
#define CEC_RX_DATA_BIT_MASK		0x1

#define CEC_BLOCK_STATUS_OK		0
#define CEC_BLOCK_STATUS_ERROR_DATA	1
#define CEC_BLOCK_STATUS_ERROR_EOM	2
#define CEC_BLOCK_STATUS_ERROR_ACK	3

typedef struct 
{
	uint8_t data	: 8;
	uint8_t eom	: 1;
	uint8_t ack	: 1;
	uint8_t status	: 8;
} cec_block_t;


typedef enum
{
	CEC_ACK_OP_ACK = 0,
	CEC_ACK_OP_NACK,
	CEC_ACK_OP_RX_ACK
} cec_ack_op_t;

static inline void cec_set_pin(void)
{
	CEC_GPIO->BSRRL |= (1<<CEC_PIN);
}

static inline void cec_clr_pin(void)
{
	CEC_GPIO->BSRRH |= (1<<CEC_PIN);
}

static inline uint8_t cec_get_pin(void)
{
	return (CEC_GPIO->IDR & (1<<CEC_PIN)) != 0;
}

static void cec_wait(uint32_t v)
{
	delay_us(CEC_TIME_UNIT*v);
}

static uint8_t cec_rx_start_bit(void)
{
	int counter = 0;
	
	/* 
	 * Set High impedance 
	 */
	cec_set_pin();

	/*
	 * Measure low duration
	 */
	while(!cec_get_pin() && counter <= CEC_DURATION(START_BIT_MAX_L))
	{
		cec_wait(1);
		counter++;
	}

	/*
	 * Validate low duration
	 */ 
	if(counter < CEC_DURATION(START_BIT_MIN_L) || counter > CEC_DURATION(START_BIT_MAX_L))
	{
		return 0;
	}
	
	/*
	 * Measure total duration
	 */ 
	while(cec_get_pin() && counter <= CEC_DURATION(START_BIT_MAX_T))
	{
		cec_wait(1);
		counter++;
	}

	/*
	 * Validate total duration
	 */ 
	if(counter < CEC_DURATION(START_BIT_MIN_T) || counter > CEC_DURATION(START_BIT_MAX_T))
	{
		return 0;
	}
	
	return 1;
}

static void cec_tx_start_bit(void)
{
	cec_clr_pin();

	cec_wait(CEC_DURATION(START_BIT_NOM_L));

	cec_set_pin();

	cec_wait(CEC_DURATION(START_BIT_NOM_H));
}

static void cec_tx_data_bit(uint8_t val)
{
	cec_clr_pin();

	cec_wait( val ? CEC_DURATION(DATA1_BIT_NOM_L) : CEC_DURATION(DATA0_BIT_NOM_L));

	cec_set_pin();

	cec_wait( val ? CEC_DURATION(DATA1_BIT_NOM_H) : CEC_DURATION(DATA0_BIT_NOM_H));
}

static uint8_t cec_tx_ack(void)
{
	if(!cec_get_pin())
	{
		cec_tx_data_bit(0);
		
		cec_clr_pin();
		
		cec_wait(1);

		cec_set_pin();

		return 1;
	}
	else
	{
		return 0;
	}
}

static uint8_t cec_rx_data_bit(void)
{
	int counter = 0;
	int rising_edge = 0;
	uint8_t data = 0;
	/* 
	 * Set High impedance 
	 */
	cec_set_pin();

	while(counter <= CEC_DURATION(DATAx_BIT_MAX_T))
	{
		cec_wait(1);
		counter++;

		/*
		 * Store rising edge time
		 */ 
		if(!rising_edge && cec_get_pin())
		{
			rising_edge = counter;
		}

		/*
		 * Falling edge means end of data bit
		 */ 
		if(rising_edge && !cec_get_pin())
		{
			break;
		}

		/*
		 * Nominal sample time
		 */  
		if(CEC_DURATION(DATAx_BIT_NOM_S) == counter)
		{
			data = cec_get_pin();
		}
	}

	/*
	 * Validate low duration
	 */ 
	if(data)
	{
		if(rising_edge < CEC_DURATION(DATA1_BIT_MIN_L) || rising_edge > CEC_DURATION(DATA1_BIT_MAX_L))
		{
			data |= CEC_RX_DATA_BIT_ERROR|CEC_RX_DATA_BIT_ERROR_L1;
		}
	}
	else
	{
		if(rising_edge < CEC_DURATION(DATA0_BIT_MIN_L) || rising_edge > CEC_DURATION(DATA0_BIT_MAX_L))
		{
			data |= CEC_RX_DATA_BIT_ERROR|CEC_RX_DATA_BIT_ERROR_L0;
		}
	}
	
	/*
	 * Validate total duration
	 */
	if(counter > CEC_DURATION(DATAx_BIT_MAX_T) || counter < CEC_DURATION(DATAx_BIT_MIN_T))
	{
		data |= CEC_RX_DATA_BIT_ERROR|CEC_RX_DATA_BIT_ERROR_T;
	}


	return data;
}

static uint8_t cec_rx_ack(uint8_t eom)
{
	uint8_t data = cec_rx_data_bit();
	if(data&CEC_RX_DATA_BIT_ERROR)
	{
		if(eom)
		{
			if((data&CEC_RX_DATA_BIT_ERROR_T) && !(data&CEC_RX_DATA_BIT_ERROR_L0) && !(data&CEC_RX_DATA_BIT_ERROR_L1))
			{
				data &= CEC_RX_DATA_BIT_MASK;

				return data ? 0 : 1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return data ? 0 : 1;
	}
}


static cec_block_t cec_rx_block(cec_ack_op_t ack)
{
	cec_block_t block = {
		.data = 0,
		.eom = 0,
		.ack = 0,
		.status = CEC_BLOCK_STATUS_OK
	};

	uint8_t data = CEC_RX_DATA_BIT_ERROR;

	for(int i=0;i<8;i++)
	{
		block.data <<= 1;
		
		data = cec_rx_data_bit();

		if(data&CEC_RX_DATA_BIT_ERROR)
		{
			block.status = CEC_BLOCK_STATUS_ERROR_DATA;
		}

		block.data |= data;
	}

	data = cec_rx_data_bit();
	if(data&CEC_RX_DATA_BIT_ERROR)
	{
		block.status = CEC_BLOCK_STATUS_ERROR_EOM;
	}

	block.eom = data;

	if(CEC_ACK_OP_ACK == ack)
	{
		if(CEC_BLOCK_STATUS_OK == block.status)
		{
			block.ack = cec_tx_ack();
			if(!block.ack)
			{
				block.status = CEC_BLOCK_STATUS_ERROR_ACK;
			}
		}
	}
	else if(CEC_ACK_OP_RX_ACK == ack)
	{
		data = cec_rx_ack(block.eom);
		if(data&CEC_RX_DATA_BIT_ERROR)
		{
			block.status = CEC_BLOCK_STATUS_ERROR_ACK;
		}

		block.ack = data;
	}

	return block;
}

static inline cec_block_t cec_rx_header_block(void)
{
	return cec_rx_block(CEC_ACK_OP_NACK);
}

static inline cec_block_t cec_rx_data_block(void)
{
	return cec_rx_block(CEC_ACK_OP_ACK);
}

static inline cec_block_t cec_monitor_data_block(void)
{
	return cec_rx_block(CEC_ACK_OP_RX_ACK);
}

static inline int cec_rx_filter_ack(struct cec_rx_filter * rxf, uint8_t follower)
{
	return (rxf->receive.all&(1<<follower)) != 0;
}

static inline int cec_rx_filter_monitor(struct cec_rx_filter * rxf, struct cec_message_header header)
{
	return (rxf->monitor.initiator.all&(1<<header.initiator)) != 0 ||
	       (rxf->monitor.follower.all&(1<<header.follower)) != 0;	
}

static void cec_init_gpio(void)
{
	/*
	 * Mode Output
	 */
	CEC_GPIO->MODER &=~(0x3<<(CEC_PIN<<1));
	CEC_GPIO->MODER |= (0x1<<(CEC_PIN<<1));
	
	/*
	 * Open Draing output
	 */
	CEC_GPIO->OTYPER |= (1<<CEC_PIN);

	/*
	 * High Speed
	 */ 
	CEC_GPIO->OSPEEDR &=~(0x3<<(CEC_PIN<<1));
	CEC_GPIO->OSPEEDR |= (0x3<<(CEC_PIN<<1));

	/*
	 * Push-up Push-down 
	 */
	CEC_GPIO->PUPDR &=~(0x3<<(CEC_PIN<<1));
}

static void cec_init_nvic(void)
{
	/*
	 * Set IRQ priority
	 */
	NVIC_SetPriority(CEC_EXTI, CEC_EXTI_PRIORITY);

	/*
	 * Enable IRQ
	 */
	NVIC_EnableIRQ(CEC_EXTI);
}

static void cec_init_exti(void)
{
	/*
	 * Connect EXTI line to CEC GPIO Pin
	 */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[CEC_EXTICR] &=~(0xf<<CEC_EXTI_LINE);
	SYSCFG->EXTICR[CEC_EXTICR] |= (CEC_EXTI_PIN<<CEC_EXTI_LINE);

	/*
	 * Mask interrupt
	 */
	EXTI->IMR |= CEC_EXTI_IMR;

	/*
	 * Rising edge
	 */
	EXTI->RTSR |= CEC_EXTI_RTSR;

	/*
	 * Falling edge
	 */
	EXTI->FTSR |= CEC_EXTI_FTSR;

	/*
	 * Clear pending interrupt
	 */
	EXTI->PR |= CEC_EXTI_PR;
}

void cec_init(void)
{
	/*
	 * Enable Peripheral clock for GPIO
	 */
	RCC->CEC_PIN_ENR |= CEC_PIN_ENR_GPIO;
	
	/*
	 * Go to High Impedance
	 */
	cec_set_pin();

	/*
	 * Configure GPIO
	 */
	cec_init_gpio();

	/*
	 * Configure NVIC
	 */
	cec_init_nvic();

	/*
	 * Configure EXTI
	 */
	cec_init_exti();
}

cec_result_t cec_rx_message(struct cec_rx_message * msg, struct cec_rx_filter * rxf)
{
	cec_block_t block;
	
	if(!msg || !rxf)
	{
		return CEC_ERROR_PARAM;
	}

	if(!cec_rx_start_bit())
	{
		return CEC_ERROR_RX_START_BIT;
	}

	block = cec_rx_header_block();
	if(CEC_BLOCK_STATUS_OK != block.status)
	{
		return CEC_ERROR_RX_HEADER_BLOCK;
	}

	msg->header.initiator = block.data>>4;
	msg->header.follower = block.data&0xF;
	
	int monitor = 0;
	int ret = CEC_RESULT_OK;

	if(cec_rx_filter_ack(rxf, msg->header.follower))
	{
		msg->ack = cec_tx_ack();
	}
	else
	{
		if(cec_rx_filter_monitor(rxf, msg->header))
		{
			msg->ack = cec_rx_ack(block.eom);
			monitor = 1;
		}
		else
		{
			ret = CEC_ERROR_RX_DROPPED;
		}
	}

	int len = 0;
	while(!block.eom)
	{
		if(!monitor)
		{
			block = cec_rx_data_block();
			if(CEC_BLOCK_STATUS_OK == block.status)
			{

				msg->message.buff[len] = block.data;
				len++;
			}
			else
			{
				ret = CEC_ERROR_RX_DATA_BLOCK;
				break;
			}
		}
		else
		{
			block = cec_monitor_data_block();
			if(CEC_BLOCK_STATUS_OK == block.status)
			{
				msg->message.buff[len] = block.data;
				len++;
				msg->ack = block.ack;
			}
			else
			{
				ret = CEC_ERROR_RX_DATA_BLOCK;
				break;
			}
		}
	}

	msg->length = len;

	return ret;

}


/**
 * @}
 */
