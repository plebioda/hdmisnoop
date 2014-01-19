#include <cec.h>

const char * cec_opcode_to_str(uint8_t opcode)
{
	switch(opcode)
	{
		case CEC_OPCODE_FEATURE_ABORT:
			return "Feature Abort";
		case CEC_OPCODE_IMAGE_VIEW_ON:
			return "Image View On";
		case CEC_OPCODE_TUNER_STEP_INCREMENT:
			return "Tuner Step Increment";
		case CEC_OPCODE_TUNER_STEP_DECREMENT:
			return "Tuner Step Decrement";
		case CEC_OPCODE_TUNER_DEVICE_STATUS:
			return "Tuner Device Status";
		case CEC_OPCODE_GIVE_TUNER_DEVICE_STATUS:
			return "Give Tuner Device Status";
		case CEC_OPCODE_RECORD_ON:
			return "Record On";
		case CEC_OPCODE_RECORD_STATUS:
			return "Record Status";
		case CEC_OPCODE_RECORD_OFF:
			return "Record Off";
		case CEC_OPCODE_TEXT_VIEW_ON:
			return "Text View On";
		case CEC_OPCODE_RECORD_TV_SCREEN:
			return "Record TV Screen";
		case CEC_OPCODE_GIVE_DECK_STATUS:
			return "Give Deck Status";
		case CEC_OPCODE_DECK_STATUS:
			return "Deck Status";
		case CEC_OPCODE_SET_MENU_LANGUAGE:
			return "Set Menu Language";
		case CEC_OPCODE_CLEAR_ANALOG_TIMER:
			return "Clear Analog Timer";
		case CEC_OPCODE_SET_ANALOG_TIMER:
			return "Set Analog Timer";
		case CEC_OPCODE_TIMER_STATUS:
			return "Timer Status";
		case CEC_OPCODE_STANDBY:
			return "Standby";
		case CEC_OPCODE_PLAY:
			return "Play";
		case CEC_OPCODE_DECK_CONTROL:
			return "Deck Control";
		case CEC_OPCODE_TIMER_CLEARED_STATUS:
			return "Timer Cleared Status";
		case CEC_OPCODE_USER_CONTROL_PRESSED:
			return "User Control Pressed";
		case CEC_OPCODE_USER_CONTROL_RELEASED:
			return "User Control Released";
		case CEC_OPCODE_GIVE_OSD_NAME:
			return "Give OSD Name";
		case CEC_OPCODE_SET_OSD_NAME:
			return "Set OSD Name";
		case CEC_OPCODE_SET_OSD_STRING:
			return "Set OSD String";
		case CEC_OPCODE_SET_TIMER_PROGRAM_TITLE:
			return "Set Timer Program Title";
		case CEC_OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
			return "System Audio Mode Request";
		case CEC_OPCODE_GIVE_AUDIO_STATUS:
			return "Give Audio Status";
		case CEC_OPCODE_SET_SYSTEM_AUDIO_MODE:
			return "System Audio Mode";
		case CEC_OPCODE_REPORT_AUDIO_STATUS:
			return "Report Audio Status";
		case CEC_OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
			return "Give System Audio Mode Status";
		case CEC_OPCODE_SYSTEM_AUDIO_MODE_STATUS:
			return "System Audio Mode Status";
		case CEC_OPCODE_ROUTING_CHANGE:
			return "Routing Change";
		case CEC_OPCODE_ROUTING_INFORMATION:
			return "Routing Information";
		case CEC_OPCODE_ACTIVE_SOURCE:
			return "Active Source";
		case CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
			return "Give Physical Address";
		case CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:
			return "Report Physical Address";
		case CEC_OPCODE_REQUEST_ACTIVE_SOURCE:
			return "Request Active Source";
		case CEC_OPCODE_SET_STREAM_PATH:
			return "Set Stream Path";
		case CEC_OPCODE_DEVICE_VENDOR_ID:
			return "Device Vendor ID";
		case CEC_OPCODE_VENDOR_COMMAND:
			return "Vendor Command";
		case CEC_OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
			return "Vendor Remote Button Down";
		case CEC_OPCODE_VENDOR_REMOTE_BUTTON_UP:
			return "Vendor Remote Button Up";
		case CEC_OPCODE_GIVE_DEVICE_VENDOR_ID:
			return "Give Device Vendor ID";
		case CEC_OPCODE_MENU_REQUEST:
			return "Menu Request";
		case CEC_OPCODE_MENU_STATUS:
			return "Menu Status";
		case CEC_OPCODE_GIVE_DEVICE_POWER_STATUS:
			return "Give Device Power Status";
		case CEC_OPCODE_REPORT_POWER_STATUS:
			return "Report Power Status";
		case CEC_OPCODE_GET_MENU_LANGUAGE:
			return "Get Menu Language";
		case CEC_OPCODE_SELECT_ANALOG_SERVICE:
			return "Select Analog Service";
		case CEC_OPCODE_SELECT_DIGITAL_SERVICE:
			return "Select Digital Service";
		case CEC_OPCODE_SET_DIGITAL_TIMER:
			return "Set Digital Timer";
		case CEC_OPCODE_CLEAR_DIGITAL_TIMER:
			return "Clear Digital Timer";
		case CEC_OPCODE_SET_AUDIO_RATE:
			return "Set Audio Rate";
		case CEC_OPCODE_INACTIVE_SOURCE:
			return "Inactive Source";
		case CEC_OPCODE_CEC_VERSION:
			return "CEC Version";
		case CEC_OPCODE_GET_CEC_VERSION:
			return "Get CEC Version";
		case CEC_OPCODE_VENDOR_COMMAND_WITH_ID:
			return "Vendor Command With ID";
		case CEC_OPCODE_CLEAR_EXTERNAL_TIMER:
			return "Clear External Timer";
		case CEC_OPCODE_SET_EXTERNAL_TIMER:
			return "Set External Timer";
		case CEC_OPCODE_REPORT_SHORT_AUDIO_DESCRIPTOR:
			return "Report Short Audio Descriptor";
		case CEC_OPCODE_REQUEST_SHORT_AUDIO_DESCRIPTOR:
			return "Request Short Audio Descriptor";
		case CEC_OPCODE_INIT_ARC:
			return "Init ARC";
		case CEC_OPCODE_REPORT_ARC_INITED:
			return "Report ARC Inited";
		case CEC_OPCODE_REPORT_ARC_TERMINATED:
			return "Report ARC Terminated";
		case CEC_OPCODE_REQUEST_ARC_INIT:
			return "Request ARC Init";
		case CEC_OPCODE_REQUEST_ARC_TERMINATION:
			return "Request ARC Termination";
		case CEC_OPCODE_TERMINATE_ARC:
			return "Terminate ARC";
		case CEC_OPCODE_CDC:
			return "CDC";
		case CEC_OPCODE_ABORT:
			return "Abort";
		default:
			return "Unknown";
	}
}

