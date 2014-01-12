#include <sys/stat.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <errno.h>
#include <clock.h>

#ifndef USART_BAUD_RATE
#define USART_BAUD_RATE	115200
#endif

void _init(void)
{
	struct system_clock_frequency freq;
	system_clock_get_frequency(&freq);

	RCC->APB1ENR |= (1<<17); /*USART2 Clock Enable*/
	RCC->AHB1ENR |= (1<<0); /*GPIO A Clock Enable*/
	GPIOA->MODER |= (0x2<<(2*2))|(0x2<<(3*2));
	GPIOA->PUPDR |= (0x1<<(2*2))|(0x1<<(3*2));
	GPIOA->AFR[0] |= (0x7<<(2*4))|(0x7<<(3*4));
#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)

	/*
	 * USARTDIV = fck / (8 * (2-OVER8) * BAUD)
	 */
	uint32_t usartdiv = 100*freq.PCLK1/(16*USART_BAUD_RATE);
	uint32_t div_mantissa = usartdiv/100;
	uint32_t fraction = usartdiv%100;
	uint32_t div_fraction = ((fraction*16)+50)/100;

	uint32_t BRR = (div_mantissa<<4)|(div_fraction&0xf);
	USART2->BRR = BRR;
	USART2->CR1 |= USART_CR1_UE|USART_Mode_Tx|USART_Mode_Rx;
	printf("\n");
}

int _close(int file) {
	return 0;
}

int _fstat(int file, struct stat *st) {
	return 0;
}

int _isatty(int file) {
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _open(const char *name, int flags, int mode) {
	return -1;
}

int _read(int file, char *ptr, int len) {
	return 0;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int _getpid ()
{
	return 1;
}

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");

caddr_t _sbrk_r (struct _reent *r, int incr)
{
	extern char   end asm ("end"); /* Defined by the linker.  */
	static char * heap_end;
	char *        prev_heap_end;

	if (heap_end == NULL)
		heap_end = & end;

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr)
	{
		/* Some of the libstdc++-v3 tests rely upon detecting
        out of memory errors, so do not abort here.  */
#if 0
		extern void abort (void);

		_write (1, "_sbrk: Heap and stack collision\n", 32);

		abort ();
#else
		//errno = ENOMEM;
		return (caddr_t) -1;
#endif
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

void usart_send_data(char d)
{
#define USART_FLAG_TC                        ((uint16_t)0x0040)
	USART2->DR = d;
	while(! (USART2->SR & USART_FLAG_TC) );
}

int _write(int file, char *ptr, int len) {
	int i;

	for(i = 0;i < len;i++)
	{
#ifdef USART_ADD_CRLF
		if(ptr[i] == '\n')
		{
			usart_send_data('\r');
		}
#endif
		usart_send_data(ptr[i]);
	}

	return len;
}

void hard_fault_handler_c (unsigned int * hardfault_args)
{
  unsigned int stacked_r0;
  unsigned int stacked_r1;
  unsigned int stacked_r2;
  unsigned int stacked_r3;
  unsigned int stacked_r12;
  unsigned int stacked_lr;
  unsigned int stacked_pc;
  unsigned int stacked_psr;
 
  stacked_r0 = ((unsigned long) hardfault_args[0]);
  stacked_r1 = ((unsigned long) hardfault_args[1]);
  stacked_r2 = ((unsigned long) hardfault_args[2]);
  stacked_r3 = ((unsigned long) hardfault_args[3]);
 
  stacked_r12 = ((unsigned long) hardfault_args[4]);
  stacked_lr = ((unsigned long) hardfault_args[5]);
  stacked_pc = ((unsigned long) hardfault_args[6]);
  stacked_psr = ((unsigned long) hardfault_args[7]);
 
  printf ("\n\n[Hard fault handler - all numbers in hex]\n");
  printf ("R0 = %x\n", stacked_r0);
  printf ("R1 = %x\n", stacked_r1);
  printf ("R2 = %x\n", stacked_r2);
  printf ("R3 = %x\n", stacked_r3);
  printf ("R12 = %x\n", stacked_r12);
  printf ("LR [R14] = %x  subroutine call return address\n", stacked_lr);
  printf ("PC [R15] = %x  program counter\n", stacked_pc);
  printf ("PSR = %x\n", stacked_psr);
  printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
  printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
  printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
  printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
  printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));
  printf ("SCB_SHCSR = %x\n", SCB->SHCSR);
 
  while (1);
}
