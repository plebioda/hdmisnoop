#include <cec.h>

const char * cec_opcode_to_str(cec_opcode_t opcode)
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


const char * cec_device_type_to_str(cec_device_type_t type)
{
	switch(type)
	{
		case CEC_DEVICE_TYPE_TV:
			return "TV";
		case CEC_DEVICE_TYPE_RECORDING_DEVICE:
			return "Recording Device";
		case CEC_DEVICE_TYPE_RESERVED:
			return "Reserved";
		case CEC_DEVICE_TYPE_STB:
			return "STB";
		case CEC_DEVICE_TYPE_PLAYBACK:
			return "Playback";
		case CEC_DEVICE_TYPE_AUDIO:
			return "Audio";
		case CEC_DEVICE_TYPE_SWITCH:
			return "Switch";
		case CEC_DEVICE_TYPE_VIDEO_PROCESSOR:
			return "Video Processor";
		default:
			return "Unknown";
	}
}


const char * cec_abort_reason_to_str(cec_abort_reason_t reason)
{
	switch(reason)
	{
		case CEC_ABORT_REASON_UNRECOGNIZED_OPCODE:
			return "Unrecognized opcode";
		case CEC_ABORT_REASON_NOT_IN_CORRECT_MODE_TO_RESPOND:
			return "Not in correct mode to respond";
		case CEC_ABORT_REASON_CANNOT_PROVIDE_SOURCE:
			return "Cannot provide source";
		case CEC_ABORT_REASON_INVALID_OPERAND:
			return "Invalid operand";
		case CEC_ABORT_REASON_REFUSED:
			return "Refused";
		default:
			return "Unknown";
	}
}

const char * cec_menu_request_type_to_str(cec_menu_request_type_t req)
{
	switch(req)
	{
		case CEC_MENU_REQUEST_TYPE_ACTIVATE:
			return "Activate";
		case CEC_MENU_REQUEST_TYPE_DEACTIVATE:
			return "Deactivate";
		case CEC_MENU_REQUEST_TYPE_QUERY:
			return "Query";
		default:
			return "Unknown";
	}
}

const char * cec_menu_state_to_str(cec_menu_state_t state)
{
	switch(state)
	{
		case CEC_MENU_STATE_ACTIVATED:
			return "Activated";
		case CEC_MENU_STATE_DEACTIVATED:
			return "Deactivated";
		default:
			return "Unknown";
	}
}

const char * cec_power_status_to_str(cec_power_status_t status)
{
	switch(status)
	{
		case CEC_POWER_STATUS_ON:
			return "On";
		case CEC_POWER_STATUS_STANDBY:
			return "Standby";
		case CEC_POWER_STATUS_IN_TRANSITION_STANDBY_TO_ON:
			return "In transition Standby to On";
		case CEC_POWER_STATUS_IN_TRANSITION_ON_TO_STANDBY:
			return "In transition On to Standby";
		default:
			return "Unknown";
	}
}

const char * cec_logical_address_to_str(cec_logical_address_t addr)
{
	switch(addr)
	{
		case CEC_LOGICAL_ADDRESS_TV:
			return "TV";
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_1:
			return "Recording Device 1";
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_2:
			return "Recording Device 2";
		case CEC_LOGICAL_ADDRESS_STB_1:
			return "STB 1";
		case CEC_LOGICAL_ADDRESS_PLAYBACK_1:
			return "Playback 1";
		case CEC_LOGICAL_ADDRESS_AUDIO_SYSTEM:
			return "Audio System";
		case CEC_LOGICAL_ADDRESS_STB_2:
			return "STB 2";
		case CEC_LOGICAL_ADDRESS_STB_3:
			return "STB 3";
		case CEC_LOGICAL_ADDRESS_PLAYBACK_2:
			return "Playback 2";
		case CEC_LOGICAL_ADDRESS_RECORDING_DEVICE_3:
			return "Recording Device 3";
		case CEC_LOGICAL_ADDRESS_FREE_USE:
			return "Free Use";
		case CEC_LOGICAL_ADDRESS_UREGISTERED:
			return "Unregistered";
		case CEC_LOGICAL_ADDRESS_BROADCAST:
			return "Broadcast";
		default:
			return "Unknown";
	}
}

const char * cec_user_control_to_str(cec_user_control_t uc)
{
	switch(uc)
	{
		case CEC_USER_CONTROL_Select:
			return "Select";
		case CEC_USER_CONTROL_Up:
			return "Up";
		case CEC_USER_CONTROL_Down:
			return "Down";
		case CEC_USER_CONTROL_Left:
			return "Left";
		case CEC_USER_CONTROL_Right:
			return "Right";
		case CEC_USER_CONTROL_RightUp:
			return "Right-Up";
		case CEC_USER_CONTROL_RightDown:
			return "Right-Down";
		case CEC_USER_CONTROL_LeftUp:
			return "Left-Up";
		case CEC_USER_CONTROL_LeftDown:
			return "Left-Down";
		case CEC_USER_CONTROL_RootMenu:
			return "Root Menu";
		case CEC_USER_CONTROL_SetupMenu:
			return "Setup Menu";
		case CEC_USER_CONTROL_ContentsMenu:
			return "Contents Menu";
		case CEC_USER_CONTROL_FavoriteMenu:
			return "Favorite Menu";
		case CEC_USER_CONTROL_Exit:
			return "Exit";
		case CEC_USER_CONTROL_Number0:
			return "0";
		case CEC_USER_CONTROL_Number1:
			return "1";
		case CEC_USER_CONTROL_Number2:
			return "2";
		case CEC_USER_CONTROL_Number3:
			return "3";
		case CEC_USER_CONTROL_Number4:
			return "4";
		case CEC_USER_CONTROL_Number5:
			return "5";
		case CEC_USER_CONTROL_Number6:
			return "6";
		case CEC_USER_CONTROL_Number7:
			return "7";
		case CEC_USER_CONTROL_Number8:
			return "8";
		case CEC_USER_CONTROL_Number9:
			return "9";
		case CEC_USER_CONTROL_Dot:
			return "Dot";
		case CEC_USER_CONTROL_Enter:
			return "Enter";
		case CEC_USER_CONTROL_Clear:
			return "Clear";
		case CEC_USER_CONTROL_ChannelUp:
			return "Channel Up";
		case CEC_USER_CONTROL_ChannelDown:
			return "Channel Down";
		case CEC_USER_CONTROL_PreviousChannel:
			return "Previous Channel";
		case CEC_USER_CONTROL_SoundSelect:
			return "Sound Select";
		case CEC_USER_CONTROL_InputSelect:
			return "Input Select";
		case CEC_USER_CONTROL_DisplayInformation:
			return "Display Information";
		case CEC_USER_CONTROL_Help:
			return "Help";
		case CEC_USER_CONTROL_PageUp:
			return "Page Up";
		case CEC_USER_CONTROL_PageDown:
			return "Page Down";
		case CEC_USER_CONTROL_Power:
			return "Power";
		case CEC_USER_CONTROL_VolumeUp:
			return "Volume Up";
		case CEC_USER_CONTROL_VolumeDown:
			return "Volume Down";
		case CEC_USER_CONTROL_Mute:
			return "Mute";
		case CEC_USER_CONTROL_Play:
			return "Play";
		case CEC_USER_CONTROL_Stop:
			return "Stop";
		case CEC_USER_CONTROL_Pause:
			return "Pause";
		case CEC_USER_CONTROL_Record:
			return "Record";
		case CEC_USER_CONTROL_Rewind:
			return "Rewind";
		case CEC_USER_CONTROL_FastForward:
			return "Fast Forward";
		case CEC_USER_CONTROL_Eject:
			return "Eject";
		case CEC_USER_CONTROL_Forward:
			return "Forward";
		case CEC_USER_CONTROL_Backward:
			return "Backward";
		case CEC_USER_CONTROL_Angle:
			return "Angle";
		case CEC_USER_CONTROL_Subpicture:
			return "Sub picture";
		case CEC_USER_CONTROL_VideoOnDemand:
			return "VOD";
		case CEC_USER_CONTROL_EPG:
			return "EPG";
		case CEC_USER_CONTROL_TimerProgramming:
			return "TImer Programming";
		case CEC_USER_CONTROL_InitialConfig:
			return "Initial Config";
		case CEC_USER_CONTROL_PlayFunction:
			return "Play Function";
		case CEC_USER_CONTROL_PausePlayFunction:
			return "Pause-Play Function";
		case CEC_USER_CONTROL_RecordFunction:
			return "Record Function";
		case CEC_USER_CONTROL_PauseRecordFunction:
			return "Pause Record Function";
		case CEC_USER_CONTROL_StopFunction:
			return "Stop Function";
		case CEC_USER_CONTROL_MuteFunction:
			return "Mute Function";
		case CEC_USER_CONTROL_RestoreVolumeFunction:
			return "Restore Volume Function";
		case CEC_USER_CONTROL_TuneFunction:
			return "Tune Function";
		case CEC_USER_CONTROL_SelectDiskFunction:
			return "Select Disk Function";
		case CEC_USER_CONTROL_SelectAVInputFunction:
			return "Select A/V Input Function";
		case CEC_USER_CONTROL_SelectAudioInputFunction:
			return "Select Audio Input Function";
		case CEC_USER_CONTROL_F1Blue:
			return "F1 (Blue)";
		case CEC_USER_CONTROL_F2Red:
			return "F2 (Red)";
		case CEC_USER_CONTROL_F3Green:
			return "F3 (Green)";
		case CEC_USER_CONTROL_F4Yellow:
			return "F4 (Yellow)";
		case CEC_USER_CONTROL_F5:
			return "F5";
		default:
			if(uc>CEC_USER_CONTROL_MAX)
			{
				return "Unknown";
			}
			else
			{
				return "Reserved";
			}
	}
}


const char * cec_display_control_to_str(cec_display_control_t control)
{
	switch(control)
	{
	case CEC_DISPLAY_CONTROL_DEFAULT_TIME:
		return "Display for default time";
	case CEC_DISPLAY_CONTROL_UNTIL_CLEARED:
		return "Display until cleared";
	case CEC_DISPLAY_CONTROL_CLEAR_PREV_MSG:
		return "Clear previous message";
	case CEC_DISPLAY_CONTROL_RESERVED:
		return "Reserved";
	default:
		return "Unknown";
	}
}

const char * cec_result_to_str(cec_result_t res)
{
	switch(res)
	{
		case CEC_RESULT_OK:
			return "CEC_RESULT_OK";
		case CEC_ERROR_PARAM:
			return "CEC_ERROR_PARAM";
		case CEC_ERROR_RX_DROPPED:
			return "CEC_ERROR_RX_DROPPED";
		case CEC_ERROR_RX_START_BIT:
			return "CEC_ERROR_RX_START_BIT";
		case CEC_ERROR_RX_HEADER_BLOCK:
			return "CEC_ERROR_RX_HEADER_BLOCK";
		case CEC_ERROR_RX_DATA_BLOCK:
			return "CEC_ERROR_RX_DATA_BLOCK";
		case CEC_ERROR_RX_MONITOR_NACK:
			return "CEC_ERROR_RX_MONITOR_NACK";
		case CEC_ERROR_RX_MONITOR_DATA_BLOCK:
			return "CEC_ERROR_RX_MONITOR_DATA_BLOCK";
		default:
			"Unknown";
	}
}

