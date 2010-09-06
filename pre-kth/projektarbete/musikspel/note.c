#include "note.h"

void note_init(note_t *note, u32 timestamp, u16 type)	
{
	note->timestamp = timestamp;
	note->note_flags = 0;
}
