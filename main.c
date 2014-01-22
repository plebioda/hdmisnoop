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
				.STB1 = 1
			}
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
	if(EXTI->PR & CEC_EXTI_PR)
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
			if(msg.length > 0)
			{
				printf("| <%s> ", cec_opcode_to_str(msg.message.data.opcode));
				if(CEC_OPCODE_ACTIVE_SOURCE == msg.message.data.opcode)
				{
					printf("[%d.%d.%d.%d] ", 
						msg.message.data.operand.active_source.physical_address.A,
						msg.message.data.operand.active_source.physical_address.B,
						msg.message.data.operand.active_source.physical_address.C,
						msg.message.data.operand.active_source.physical_address.D
					      );
				}
				else if(CEC_OPCODE_ROUTING_INFORMATION == msg.message.data.opcode)
				{
					printf("[%d.%d.%d.%d] ", 
						msg.message.data.operand.routing_information.physical_address.A,
						msg.message.data.operand.routing_information.physical_address.B,
						msg.message.data.operand.routing_information.physical_address.C,
						msg.message.data.operand.routing_information.physical_address.D
					      );

				}
				else if(CEC_OPCODE_SET_STREAM_PATH == msg.message.data.opcode)
				{
					printf("[%d.%d.%d.%d] ", 
						msg.message.data.operand.set_stream_path.physical_address.A,
						msg.message.data.operand.set_stream_path.physical_address.B,
						msg.message.data.operand.set_stream_path.physical_address.C,
						msg.message.data.operand.set_stream_path.physical_address.D
					      );

				}
				else if(CEC_OPCODE_ROUTING_CHANGE == msg.message.data.opcode)
				{
					printf("[%d.%d.%d.%d -> %d.%d.%d.%d] ", 
						msg.message.data.operand.routing_change.original_address.A,
						msg.message.data.operand.routing_change.original_address.B,
						msg.message.data.operand.routing_change.original_address.C,
						msg.message.data.operand.routing_change.original_address.D,
						msg.message.data.operand.routing_change.new_address.A,
						msg.message.data.operand.routing_change.new_address.B,
						msg.message.data.operand.routing_change.new_address.C,
						msg.message.data.operand.routing_change.new_address.D
					      );

				}
				else if(CEC_OPCODE_REPORT_PHYSICAL_ADDRESS == msg.message.data.opcode)
				{
					printf("[%d.%d.%d.%d] [%s] ", 
						msg.message.data.operand.report_physical_address.physical_address.A,
						msg.message.data.operand.report_physical_address.physical_address.B,
						msg.message.data.operand.report_physical_address.physical_address.C,
						msg.message.data.operand.report_physical_address.physical_address.D,
						cec_device_type_to_str(msg.message.data.operand.report_physical_address.device_type)
					      );

				}
				else if(CEC_OPCODE_REPORT_POWER_STATUS == msg.message.data.opcode)
				{
					printf("[%s] ", cec_power_status_to_str(msg.message.data.operand.power_status));
				}
				else if(CEC_OPCODE_MENU_REQUEST == msg.message.data.opcode)
				{
					printf("[%s] ", cec_menu_request_type_to_str(msg.message.data.operand.menu_request_type));
				}
				else if(CEC_OPCODE_MENU_STATUS == msg.message.data.opcode)
				{
					printf("[%s] ", cec_menu_state_to_str(msg.message.data.operand.menu_state));
				}
				else if(CEC_OPCODE_USER_CONTROL_PRESSED == msg.message.data.opcode)
				{
					printf("[%s] ", cec_user_control_to_str(msg.message.data.operand.user_control_pressed));
				}
				else if(CEC_OPCODE_SET_OSD_NAME == msg.message.data.opcode)
				{
					msg.message.data.operand.buff[msg.length-1] = '\0';
					printf("[%s] ", (const char*)msg.message.data.operand.buff);
				}
				else if(CEC_OPCODE_SET_OSD_STRING == msg.message.data.opcode)
				{
					msg.message.data.operand.set_osd_string.string[msg.length-2] = '\0';
					printf("[%s] [%s] ", 
						cec_display_control_to_str(msg.message.data.operand.set_osd_string.display_control),
						(const char*)msg.message.data.operand.set_osd_string.string
					);

				}
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

		EXTI->PR = CEC_EXTI_PR;
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
