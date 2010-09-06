#ifndef SCENE_H
#define SCENE_H

#include "types.h"
#include "channel.h"

typedef struct
{
	u32 length; //length of session in ms.
	channel_t channel[1][5]; //1p 5 channels, sjeez temp, dynamically allocate blah blah, no time
	u32 current_note[1][5]; // Last notes played, to not have to iterate through all of them when rendering/timing
} scene_t;

extern scene_t scene;
#endif
