#ifndef CHANNEL_H
#define CHANNEL_H

#include "note.h"

#include <stdlib.h>

typedef enum
{
	CHANNEL_EMPTY = 0,
	CHANNEL_HELD
} channel_status_t;

typedef struct
{
	note_t *note_list;
	u16 num_notes;
	u16 current_note;
	channel_status_t status;
} channel_t;

void channel_init(channel_t *channel, u16 num_notes);

void channel_clear(channel_t *channel);

#endif

