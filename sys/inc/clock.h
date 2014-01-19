#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>
#include <time.h>

struct system_clock_frequency
{
	uint32_t SYSCLK;
	uint32_t HCLK;
	uint32_t PCLK1;
	uint32_t PCLK2;
};

int system_clock_get_frequency(struct system_clock_frequency * freq);

/*
 * Delay in us
 */
#define SYSTEM_CLOCK_TICK	100

void delay_init(void);
void delay_us(uint32_t n);
void delay_tick(void);


typedef struct
{
	uint64_t total_us;
	uint64_t total_ms;
	uint16_t ms;
	uint8_t seconds;
	uint8_t minutes;
	uint32_t hours;
} up_time_t;

up_time_t get_up_time(void);
void up_time_tick(time_t ticks);


#endif //_CLOCK_H
