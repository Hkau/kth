#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "types.h"

#define HAT_CENTERED	0x00
#define HAT_UP			0x01
#define HAT_RIGHT		0x02
#define HAT_DOWN		0x04
#define HAT_LEFT		0x08
#define HAT_RIGHTUP		(HAT_RIGHT | HAT_UP)
#define HAT_RIGHTDOWN	(HAT_RIGHT | HAT_DOWN)
#define HAT_LEFTUP		(HAT_LEFT | HAT_UP)
#define HAT_LEFTDOWN	(HAT_LEFT | HAT_DOWN)

typedef enum
{
	BUTTON_NONE,
	BUTTON_CHANNEL,
	BUTTON_STRUM
} button_type_t;

typedef enum
{
	BUTTON_UP = 0,
	BUTTON_DOWN = 1
} button_state;

typedef struct
{
	button_type_t type;
	u8 button;
	button_state state;
} controller_event_t;

typedef struct
{
	char *name;

	controller_event_t (*convert_axis)(u8 axis, s16 state);
	controller_event_t (*convert_button)(u8 button, u8 state);
	controller_event_t (*convert_hat)(u8 hat, u8 state);
} joystick_t;

// Internal temporary event.
extern controller_event_t temp_event;

controller_event_t null_axis(u8 axis, s16 state);
controller_event_t null_button(u8 button, u8 state);
controller_event_t null_hat(u8 hat, u8 state);

#endif
