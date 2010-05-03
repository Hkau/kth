#include <stdio.h>
#include "types.h"

#define MIDI_IDENTIFIER 0x4D546864 //MThd
#define TRACK_IDENTIFIER 0x4D54726B //MTrk

inline unsigned long ntohl(unsigned long n)
{
  return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}

inline unsigned short ntohs(unsigned short n)
{
  return ((n & 0xFF) << 8) | ((n & 0xFF00) >> 8);
}

int main(int argc, char *argv[])
{
	FILE *file = fopen("notes.mid", "rb");

	if(file == NULL)
	{
		printf("Unable to open file: %s\n", "notes.mid");
		return -1;
	}

	u8 header[14];

	fread(header, sizeof(header), 1, file);

	if(ntohl(*(u32 *)header) != MIDI_IDENTIFIER)
	{
		printf("No midi file.\n");
		return -1;
	}

	u8 tracks = *(u8 *)(header + 0x0B);

	u16 time_div = *(u16 *)(header + 0x0C);

	time_div = ntohs(time_div);

	printf("\nTracks: %d\n", tracks);

	printf("Time division: %X (%d)\n\n", time_div, time_div);

	u16 i;

	for(i = 0; i < tracks; ++i)
	{
		u8 track_time_div = time_div;
		u64 time = 0;

		//fseek(file, SEEK_CUR, 4);

		fread(header, 4, 1, file); // fseek fails for some reason, fix later

		u32 len;

		fread(&len, sizeof(u32), 1, file);

		len = ntohl(len);

		printf("Track length: %X (%d) bytes\n", len, len);

		u8 *buffer = malloc(sizeof(u8)*len);

		fread(buffer, sizeof(u8), len, file);
		
		u32 tempo = 480000;

		u32 j = 0;
		while(j < len)
		{
			u32 ticks = 0;
			u8 read_ticks;

			do
			{
				read_ticks = *(buffer + j++); // Read more of delta stamp

				ticks <<= 7;
				ticks |= (read_ticks & ~0x80);
				
			}while(read_ticks & 0x80);

			time += (tempo*ticks/* / time_div*/);

			u8 evt_type = *(buffer + j++);
			u8 last_midi_evt;

			if(evt_type < 0x80)
			{
				evt_type = last_midi_evt;
				--j;
			}

			printf("%d (%X): ", time/1000/time_div, evt_type);

			if(evt_type == 0xFF) // Meta-event
			{
					u8 meta_event = *(buffer + j++); // Time of midi event
					u8 text_len;
					u8 *text;
					printf("(evt %X) ", meta_event);
					switch(meta_event)
					{
						case 0x01:
						case 0x02:
						case 0x03:

							text_len = *(buffer + j++); // Time of midi event
							text = malloc(sizeof(u8) * text_len + 1);

							memcpy (text, buffer+j, text_len);
							j += text_len;

							text[text_len] = '\0';

							printf("Text: %s\n", text);

							free(text);

							break;

						case 0x2F:

							++j;
							printf("End of track\n\n");

							break;

						case 0x51:
							tempo = *(u32 *)(buffer + j);
							j += sizeof(u32);
							tempo &= ~0x03;
							tempo = ntohl(tempo);
							printf("Tempo: %X (%d) microseconds/quarter note\n", tempo, tempo);

							break;

						case 0x58:

							j += 5; // Skip time signature
							printf("\n");

							//++j; // Skip 0x04

							/*numerator = *(buffer + j++);
							denominator = *(buffer + j++);

							track_time_div = *(buffer + j++);

							++j; // The bb parameter expresses the number of notated 32nd notes in a MIDI quarter note (24 MIDI clocks).
								// This event allows a program to relate what MIDI thinks of as a quarter, to something entirely different.
								// Vet ej vad denna gör.

							printf("Time signature: %d/(2<<(%d-1)), %d \"MIDI clocks in a metronome click\" ??\n", numerator, denominator, track_time_div);*/


							break;

						default:
							printf("Unknown meta-event.\n");
							return -1;
					}

					continue;
			}

			last_midi_evt = evt_type;

			u8 midi_event, midi_channel;

			midi_event = (evt_type & 0xF0) >> 4;
			midi_channel = evt_type & 0x0F;


			printf("(evt %d, ch %d) ", midi_event, midi_channel);

			u8 note;
			switch(midi_event)
			{
				case 8:
					note = *(buffer + j++);
					++j;
					printf("%d up\n", note);
					break;
				case 9:
					note = *(buffer + j++);
					++j;
					printf("%d down\n", note);
					break;
				default:

					printf("(char %d) Unknown MIDI event\n", j);

					return -1;
			}
		}

		free(buffer);
	}

	fclose(file);

	return 0;
}
