#include "xplorer.h"

controller_event_t xplorer_button(u8 button, u8 state)
{
	temp_event.type = BUTTON_CHANNEL;
	temp_event.state = state;

	if(button < 2)
	{
		temp_event.button = button;
		return temp_event;
	}

	if(button == 3)
	{
		temp_event.button = 2;
		return temp_event;
	}

	if(button == 4)
	{
		temp_event.button = 3;
		return temp_event;
	}

	if(button == 6)
	{
		temp_event.button = 4;
		return temp_event;
	}

	if(button == 12 || button == 14)
	{
			temp_event.type = BUTTON_STRUM;
			return temp_event;
	}

	temp_event.type = BUTTON_NONE;

	return temp_event;
}

void controller_xplorer(joystick_t *joystick)
{
	joystick->name = "Guitar Hero X-Plorer";

	joystick->convert_axis = null_axis;
	joystick->convert_button = xplorer_button;
	joystick->convert_hat = null_hat;
}
