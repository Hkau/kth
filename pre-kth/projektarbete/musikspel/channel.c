#include "channel.h"

void channel_init(channel_t *channel, u16 num_notes)
{
	channel->num_notes = num_notes;
	if(num_notes != 0)
		channel->note_list = malloc(num_notes * sizeof(note_t));
	channel->status = CHANNEL_EMPTY;
}

void channel_clear(channel_t *channel)
{
	if(channel->num_notes != 0)
		free(channel->note_list);
}
