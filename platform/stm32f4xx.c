#include <stm32f4xx.h>
#include <stdio.h>
#include <assert.h>

void _init(void);

/* 
 * PLL Parameters 
 */

/* 
 * PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N 
 */
#define PLL_M      8
#define PLL_N      336

/*
 * SYSCLK = PLL_VCO / PLL_P 
 */
#define PLL_P      2

/* 
 * USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ 
 */
#define PLL_Q      7

#define RCC_PLLCFGR_RESERVED	((uint32_t)0x20000000)

void RCC_defaults(void)
{
	/*
	 * Set HSION bit
	 */
	RCC->CR |= RCC_CR_HSION;
	
	/*
	 * Reset CFGR register
	 */
	RCC->CFGR = 0;

	/*
	 * Reset HSEON, CSSON and PLLON bits
	 */ 
	RCC->CR &= ~(RCC_CR_HSEON|RCC_CR_CSSON|RCC_CR_PLLON);
	
	/*
	 * Reset PLLCFGR register
	 */
	RCC->PLLCFGR = RCC_PLLCFGR_RESERVED
	              |RCC_PLLCFGR_PLLQ_2
		      |RCC_PLLCFGR_PLLN_6
		      |RCC_PLLCFGR_PLLN_7
		      |RCC_PLLCFGR_PLLM_4;
	
	/*
	 * Reset HSEBYP bit
	 */ 
	RCC->CR &= ~(RCC_CR_HSEBYP);

	/*
	 * Disable all interrupts
	 */
	RCC->CIR = 0;
}

uint32_t HSE_wait_ready(void)
{
	__IO uint32_t startup_counter = 0;
	__IO uint32_t HSE_status = 0;
	
	do
	{
		HSE_status = RCC->CR & RCC_CR_HSERDY;
		startup_counter++;
	}while(!HSE_status && startup_counter < HSE_STARTUP_TIMEOUT);

	return (RCC->CR & RCC_CR_HSERDY) != 0;
}

void RCC_configure(void)
{
	/*
	 * Configure the System Clock source as PLL clocked by HSE
	 */
	
	/*
	 * Enable HSE
	 */
	RCC->CR |= RCC_CR_HSEON;
	
	/*
	 * Wait for HSE ready bit or timeout
	 */
	uint32_t HSE_ready = HSE_wait_ready();

	if(HSE_ready)
	{
		/* 
		 * Select regulator voltage output Scale 1 mode, System frequency up to 168 MHz
		 */
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;

		/* 
		 * HCLK = SYSCLK / 1
		 */
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

		/* 
		 * PCLK2 = HCLK / 2
		 */
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

		/* 
		 * PCLK1 = HCLK / 4
		 */
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

		/* 
		 * Configure the main PLL 
		 */
		RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
				(RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

		/* 
		 * Enable the main PLL
		 */
		RCC->CR |= RCC_CR_PLLON;

		/* 
		 * Wait till the main PLL is ready 
		 */
		while((RCC->CR & RCC_CR_PLLRDY) == 0);

		/* 
		 * Configure Flash prefetch, Instruction cache, Data cache and wait state 
		 */
		FLASH->ACR = FLASH_ACR_ICEN|FLASH_ACR_DCEN|FLASH_ACR_LATENCY_5WS;

		/* 
		 * Select the main PLL as system clock source 
		 */
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		/* 
		 * Wait till the main PLL is used as system clock source 
		 */
		while ((RCC->CFGR & RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL);
	}
	else
	{
		/*
		 * Initialize stdio and print assert if HSE clock 
		 * not configured correctly
		 */
		_init();
		assert(HSE_ready);
		for(;;);
	}
}

void SystemInit(void)
{
	/*
	 * Reset RCC configuration bits to the defaults
	 */
	RCC_defaults();

	/*
	 * Configure the System clock source,
	 * PLL Multiplier and Divider factors,
	 * AHB/APBx prescalers and flash settings
	 */
	RCC_configure();
}
