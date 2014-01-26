#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stm32f4xx.h>
#include <cec.h>
#include <clock.h>

#define CEC_BUFFER_SIZE		16
struct cec_message_buffer_item
{
	up_time_t time;
	cec_result_t result;
	struct cec_rx_message message;
};
struct 
{
	int size;
	int first;
	int last;
	struct cec_message_buffer_item items[CEC_BUFFER_SIZE];
} cec_message_buffer;

void EXTI0_IRQHandler(void)
{
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
		if(cec_message_buffer.size < CEC_BUFFER_SIZE)
		{
			struct cec_message_buffer_item * it = &cec_message_buffer.items[cec_message_buffer.last];
			struct cec_rx_message * msg = &it->message;
			it->result = cec_rx_message(msg, &rxf);
			it->time = get_up_time();
			cec_message_buffer.last = (cec_message_buffer.last + 1)%CEC_BUFFER_SIZE;
			cec_message_buffer.size++;
		}
		EXTI->PR = CEC_EXTI_PR;
	}
}

void init(void)
{
	delay_init();
	cec_init();
}

void print_message(cec_result_t result, up_time_t time, struct cec_rx_message * msg)
{
	if(CEC_RESULT_OK == result || CEC_ERROR_RX_DROPPED == result)
	{
		printf("| %02d:%02d:%02d.%03d | %02x -> %02x | L:%2d A:%d | ",
			time.hours,
			time.minutes,
			time.seconds,
			time.ms,
			msg->header.initiator,
			msg->header.follower,
			msg->length,
			msg->ack);

		for(int i=0;i<msg->length;i++)	
		{
			printf("0x%02x ", msg->message.buff[i]);
		}
		if(msg->length > 0)
		{
			printf("| <%s> ", cec_opcode_to_str(msg->message.data.opcode));
			if(CEC_OPCODE_ACTIVE_SOURCE == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.active_source.physical_address.A,
					msg->message.data.operand.active_source.physical_address.B,
					msg->message.data.operand.active_source.physical_address.C,
					msg->message.data.operand.active_source.physical_address.D
				      );
			}
			else if(CEC_OPCODE_ROUTING_INFORMATION == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.routing_information.physical_address.A,
					msg->message.data.operand.routing_information.physical_address.B,
					msg->message.data.operand.routing_information.physical_address.C,
					msg->message.data.operand.routing_information.physical_address.D
				      );

			}
			else if(CEC_OPCODE_SET_STREAM_PATH == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] ", 
					msg->message.data.operand.set_stream_path.physical_address.A,
					msg->message.data.operand.set_stream_path.physical_address.B,
					msg->message.data.operand.set_stream_path.physical_address.C,
					msg->message.data.operand.set_stream_path.physical_address.D
				      );

			}
			else if(CEC_OPCODE_ROUTING_CHANGE == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d -> %d.%d.%d.%d] ", 
					msg->message.data.operand.routing_change.original_address.A,
					msg->message.data.operand.routing_change.original_address.B,
					msg->message.data.operand.routing_change.original_address.C,
					msg->message.data.operand.routing_change.original_address.D,
					msg->message.data.operand.routing_change.new_address.A,
					msg->message.data.operand.routing_change.new_address.B,
					msg->message.data.operand.routing_change.new_address.C,
					msg->message.data.operand.routing_change.new_address.D
				      );

			}
			else if(CEC_OPCODE_REPORT_PHYSICAL_ADDRESS == msg->message.data.opcode)
			{
				printf("[%d.%d.%d.%d] [%s] ", 
					msg->message.data.operand.report_physical_address.physical_address.A,
					msg->message.data.operand.report_physical_address.physical_address.B,
					msg->message.data.operand.report_physical_address.physical_address.C,
					msg->message.data.operand.report_physical_address.physical_address.D,
					cec_device_type_to_str(msg->message.data.operand.report_physical_address.device_type)
				      );

			}
			else if(CEC_OPCODE_REPORT_POWER_STATUS == msg->message.data.opcode)
			{
				printf("[%s] ", cec_power_status_to_str(msg->message.data.operand.power_status));
			}
			else if(CEC_OPCODE_MENU_REQUEST == msg->message.data.opcode)
			{
				printf("[%s] ", cec_menu_request_type_to_str(msg->message.data.operand.menu_request_type));
			}
			else if(CEC_OPCODE_MENU_STATUS == msg->message.data.opcode)
			{
				printf("[%s] ", cec_menu_state_to_str(msg->message.data.operand.menu_state));
			}
			else if(CEC_OPCODE_USER_CONTROL_PRESSED == msg->message.data.opcode)
			{
				printf("[%s] ", cec_user_control_to_str(msg->message.data.operand.user_control_pressed));
			}
			else if(CEC_OPCODE_SET_OSD_NAME == msg->message.data.opcode)
			{
				msg->message.data.operand.buff[msg->length-1] = '\0';
				printf("[%s] ", (const char*)msg->message.data.operand.buff);
			}
			else if(CEC_OPCODE_SET_OSD_STRING == msg->message.data.opcode)
			{
				msg->message.data.operand.set_osd_string.string[msg->length-2] = '\0';
				printf("[%s] [%s] ", 
					cec_display_control_to_str(msg->message.data.operand.set_osd_string.display_control),
					(const char*)msg->message.data.operand.set_osd_string.string
				);

			}
		}
		if(CEC_ERROR_RX_DROPPED == result)
		{
			printf(" -> message dropped");
		}
		printf("\n");
	}
	else
	{
		printf("| %02d:%02d:%02d.%03d | %02x -> %02x | L:%2d A:%d | %s\n",
			time.hours,
			time.minutes,
			time.seconds,
			time.ms,
			msg->header.initiator,
			msg->header.follower,
			msg->length,
			msg->ack,
			cec_result_to_str(result)
		      );
	}
}

int main(void)
{
	init();
	printf("HDMI Snoop\n");
	memset(&cec_message_buffer, 0, sizeof(cec_message_buffer));
	int i=0;
	for(;;)
	{
		while(cec_message_buffer.size > 0)
		{
			struct cec_rx_message * msg = &cec_message_buffer.items[cec_message_buffer.first].message;
			cec_result_t result = cec_message_buffer.items[cec_message_buffer.first].result;
			up_time_t time = cec_message_buffer.items[cec_message_buffer.first].time;
			print_message(result, time, msg);
			cec_message_buffer.first = (cec_message_buffer.first+1)%CEC_BUFFER_SIZE;
			cec_message_buffer.size--;
		}
	}
	return 0;
}
