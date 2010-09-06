#include "controller.h"

controller_event_t temp_event;

controller_event_t null_axis(u8 axis, s16 state)
{
	temp_event.type = BUTTON_NONE;
	return temp_event;
}

controller_event_t null_button(u8 button, u8 state)
{
	temp_event.type = BUTTON_NONE;
	return temp_event;
}

controller_event_t null_hat(u8 hat, u8 state)
{
	temp_event.type = BUTTON_NONE;
	return temp_event;
}
