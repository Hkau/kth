#include "xplorer.h"

controller_event_t xplorer_button(u8 button, u8 state)
{
	if(button > 4)
	{
		temp_event.type = BUTTON_NONE;
		return temp_event;
	}

	temp_event.type = BUTTON_CHANNEL;
	temp_event.state = state;

	if(button == 3)
	{
			temp_event.button = 2;
			return temp_event;
	}
	if(button == 2)
	{
			temp_event.button = 3;
			return temp_event;
	}

	temp_event.button = button;
	return temp_event;
}

controller_event_t xplorer_hat(u8 hat, u8 state)
{
	temp_event.state = state;
	if(state == HAT_UP || state == HAT_DOWN)
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
	joystick->convert_hat = xplorer_hat;
}
