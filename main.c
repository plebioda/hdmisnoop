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
			.all = CEC_RX_FILTER_NO_DEVICES
		},
		.monitor = {
			.initiator = {
				.all = CEC_RX_FILTER_ALL_DEVICES
			},
			.follower = {
				.all = CEC_RX_FILTER_ALL_DEVICES
			},
		}
	};
	if(EXTI->PR & EXTI_PR_PR0)
	{
		cec_result_t ret = cec_rx_message(&msg, &rxf);
		if(CEC_RESULT_OK == ret || CEC_ERROR_RX_DROPPED == ret)
		{
			up_time_t uptime = get_up_time();
			printf("| %02d:%02d:%02d.%03d | %02x -> %02x | L:%2d A:%d | ",
				uptime.hours,
				uptime.minutes,
				uptime.seconds,
				uptime.ms,
				msg.header.initiator,
				msg.header.follower,
				msg.length,
				msg.ack);

			for(int i=0;i<msg.length;i++)	
			{
				printf("0x%02x ", msg.message.buff[i]);
			}
			if(CEC_ERROR_RX_DROPPED == ret)
			{
				printf(" -> message dropped");
			}
			printf("\n");
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
	cec_init();
}

int main(void)
{
	init();
	printf("HDMI Snoop\n");
	int i=0;
	for(;;)
	{
	}
	return 0;
}
