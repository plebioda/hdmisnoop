#include <clock.h>
#include <stm32f4xx.h>
#include <stdlib.h>
#include <core_cm4.h>
#include <assert.h>

#define SYSCLK_SRC_HSI	0x00
#define SYSCLK_SRC_HSE	0x04
#define SYSCLK_SRC_PLL	0x08

static __IO uint32_t delay_time;
static __IO time_t up_time;

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

void delay_init(void)
{
	struct system_clock_frequency freq;
	
	system_clock_get_frequency(&freq);
	
	/*
	 * Configure System Tick interrupt
	 */ 
	int ret = SysTick_Config(freq.SYSCLK/(1000000/SYSTEM_CLOCK_TICK));
	assert(!ret);
	if(ret)
	{
		for(;;);
	}
	
	/*
	 * Set priority
	 */
	NVIC_SetPriority(SysTick_IRQn, 0);

	/*
	 * Set Priority group settings
	 */
	SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PRIORITY_GROUP_4;
}

void delay_tick(void)
{
	if(delay_time)
	{
		delay_time--;
	}
}

void delay_us(uint32_t n)
{
	delay_time = n/SYSTEM_CLOCK_TICK;
	while(delay_time);
	delay_time = 0;
}

uint32_t system_clock_get_pll_SYSCLK(void)
{
	/* 
	 * PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
	 * SYSCLK = PLL_VCO / PLLP
	 */    
	uint32_t pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
	uint32_t pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
	uint32_t pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >>16) + 1 ) *2;
	uint32_t pllvco = 0;

	if(pllsource != 0)
	{
		/* 
		 * HSE used as PLL clock source
		 */
		pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
	}
	else
	{
		/*
		 * HSI used as PLL clock source
		 */
		pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);      
	}

	return pllvco/pllp;;
}

uint32_t system_clock_get_HCLK_prescaler(void)
{
	uint32_t presc = (RCC->CFGR & RCC_CFGR_HPRE) >> 4;
	return APBAHBPrescTable[presc];
}

uint32_t system_clock_get_PCLK1_prescaler(void)
{
	uint32_t presc = (RCC->CFGR & RCC_CFGR_PPRE1) >> 10;
	return APBAHBPrescTable[presc];
}

uint32_t system_clock_get_PCLK2_prescaler(void)
{
	uint32_t presc = (RCC->CFGR & RCC_CFGR_PPRE2) >> 13;
	return APBAHBPrescTable[presc];
}

int system_clock_get_frequency(struct system_clock_frequency * freq)
{
	if(NULL == freq)
	{
		return -1;
	}

	uint32_t sysclk_src = RCC->CFGR & RCC_CFGR_SWS;

	switch(sysclk_src)
	{
	case SYSCLK_SRC_PLL:
		freq->SYSCLK = system_clock_get_pll_SYSCLK();
		break;
	case SYSCLK_SRC_HSE:
		freq->SYSCLK = HSE_VALUE;
		break;
	case SYSCLK_SRC_HSI:
	/* fall through */
	default:
		freq->SYSCLK = HSI_VALUE;
		break;
	}

	freq->HCLK = freq->SYSCLK >> system_clock_get_HCLK_prescaler();
	freq->PCLK1 = freq->HCLK >> system_clock_get_PCLK1_prescaler();
	freq->PCLK2 = freq->HCLK >> system_clock_get_PCLK2_prescaler();
}

void up_time_tick(time_t ticks)
{
	up_time += ticks;
}

up_time_t get_up_time(void)
{
	up_time_t uptime;

	uptime.total_us = up_time;
	uptime.total_ms = uptime.total_us/1000;
	uptime.ms = uptime.total_ms%1000;
	uptime.seconds = (uptime.total_ms/1000)%60;
	uptime.minutes = (uptime.total_ms/1000/60)%60;
	uptime.hours = (uptime.total_ms/1000/60/60);

	return uptime;
}


