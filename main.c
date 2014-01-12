#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f4xx.h>
#include <cec.h>
#include <clock.h>

void EXTI0_IRQHandler(void)
{
	static struct cec_rx_message msg;
	static struct cec_rx_filter rxf = {
		.receive = {
			.device = {
				.STB1			= 1
			}
		},
		.monitor = {
			.initiator = {
				.all = 0
			},
			.follower = {
				.device = {
					.Recording_Device_2	= 1
				}
			},
		}
	};
	
	if(EXTI->PR & EXTI_PR_PR0)
	{
		cec_result_t ret = cec_rx_message(&msg, &rxf);
		if(CEC_RESULT_OK == ret)
		{
			printf("%02x -> %02x | L:%2d A:%d | ",
				msg.header.initiator,
				msg.header.follower,
				msg.length,
				msg.ack);

			for(int i=0;i<msg.length;i++)	
			{
				printf("0x%02x ", msg.message.buff[i]);
			}
			printf("\n");
		}
		else if(CEC_ERROR_RX_DROPPED == ret)
		{
			printf("%02x -> %02x | L:%2d A:%d | ",
				msg.header.initiator,
				msg.header.follower,
				msg.length,
				msg.ack);

			for(int i=0;i<msg.length;i++)	
			{
				printf("0x%02x ", msg.message.buff[i]);
			}
			printf(" -> message dropped\n");
		}
		else
		{
			printf("cec_receive_message: %d\n", ret);
		}
		memset(&msg, 0, sizeof(msg));

		EXTI->PR = EXTI_PR_PR0;
	}

}

void init(void)
{
	delay_init();
}

int main(void)
{
	printf("Hello STM32\n");
	for(;;)
	{
		printf("Tick\n");
		delay_us(1000000);
	}
	return 0;
}
