#ifndef NOTE_H
#define NOTE_H

#include "types.h"

typedef enum
{
	TYPE_NORMAL,
	TYPE_HOLD_START,
	TYPE_HOLD_END
} note_type_t;

#define NOTE_HIT 0x01

typedef struct
{
	u32	timestamp;		// timestamp in milliseconds.
	note_type_t type;	// type, activation etc. hold start.
	u32 note_flags;
} note_t;

void note_init(note_t *note, u32 timestamp,u16 type);

#endif
