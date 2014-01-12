#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>

struct system_clock_frequency
{
	uint32_t SYSCLK;
	uint32_t HCLK;
	uint32_t PCLK1;
	uint32_t PCLK2;
};

int system_clock_get_frequency(struct system_clock_frequency * freq);

#endif //_CLOCK_H
