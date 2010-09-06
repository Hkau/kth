#include "fof_song.h"

#include "sound.h"
#include "scene.h"
#include "math.h"

#define MUSIC_TRACKS 3
char *music_file_names[] = {"song.ogg", "guitar.ogg", "rhythm.ogg"};

#ifdef WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif

#define player 0 //temp

#include "types.h"

#include <stdio.h>
#include <string.h>

#define MIDI_IDENTIFIER 0x4D546864 //MThd
#define TRACK_IDENTIFIER 0x4D54726B //MTrk

#ifndef ntohl
inline unsigned long ntohl(unsigned long n)
{
	return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}
#endif

#ifndef ntohs
inline unsigned long ntohs(unsigned short n)
{
	return ((n & 0xFF) << 8) | ((n & 0xFF00) >> 8);
}
#endif

#define DIFFICULTY_OFFSET_EASY		60
#define DIFFICULTY_OFFSET_MEDIUM	72
#define DIFFICULTY_OFFSET_HARD		84
#define DIFFICULTY_OFFSET_EXPERT	96

#define DIFFICULTY_OFFSET DIFFICULTY_OFFSET_HARD

int load_mid(char *path)
{
	FILE *file = fopen(path, "rb");

	printf("%s\n", path);

	if(file == NULL)
	{
		printf("Unable to open file: %s\n", path);
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

	//printf("\nTracks: %d\n", tracks);

	//printf("Time division: %X (%d)\n\n", time_div, time_div);

	u16 i;
	
	bool guitar_track = false;

	for(i = 0; i < tracks; ++i)
	{
		u16 note_index[5] = {0, 0, 0, 0, 0};
		u8 track_time_div = time_div;
		u64 time = 0;

		//fseek(file, SEEK_CUR, 4);

		fread(header, 4, 1, file); // fseek fails for some reason, fix later

		u32 len;

		fread(&len, sizeof(u32), 1, file);

		len = ntohl(len);

		//printf("Track length: %X (%d) bytes\n", len, len);

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

			//printf("%d (%X): ", time/1000/time_div, evt_type);

			if(evt_type == 0xFF) // Meta-event
			{
					u8 meta_event = *(buffer + j++); // Time of midi event
					u8 text_len;
					u8 *text;
					//printf("(evt %X) ", meta_event);
					switch(meta_event)
					{
						case 0x01:
						case 0x02:
							text_len = *(buffer + j++); // Time of midi event
							text = malloc(sizeof(u8) * text_len + 1);

							memcpy (text, buffer+j, text_len);
							j += text_len;

							text[text_len] = '\0';

							//printf("Text: %s\n", text);

							free(text);

							break;

						case 0x03:

							text_len = *(buffer + j++); // Time of midi event
							text = malloc(sizeof(u8) * text_len + 1);

							memcpy (text, buffer+j, text_len);
							j += text_len;

							text[text_len] = '\0';
							
							if(strcmp(text, "PART GUITAR") == 0)
								guitar_track = true;

							// Init channels with the approximate max amount of notes this can contain, this *WILL* be overkill, but is a quick solution
							channel_init(&scene.channel[player][0], len/10);
							channel_init(&scene.channel[player][1], len/10);
							channel_init(&scene.channel[player][2], len/10);
							channel_init(&scene.channel[player][3], len/10);
							channel_init(&scene.channel[player][4], len/10);

							//printf("Text: %s\n", text);

							free(text);

							break;

						case 0x2F:

							if(guitar_track)
							{
								u8 k;
								for(k = 0; k < 5; ++k)
								{
									scene.channel[player][k].num_notes = note_index[k];
								}
								//printf("%d, %d, %d, %d, %d notes.\n", scene.channel[player][0].num_notes, scene.channel[player][1]. num_notes,scene.channel[player][2].num_notes, scene.channel[player][3].num_notes, scene.channel[player][4].num_notes);
								return 0;
							}
							++j;
							//printf("End of track\n\n");
							
							guitar_track = false;

							break;

						case 0x51:
							tempo = *(u32 *)(buffer + j);
							j += sizeof(u32);
							tempo &= ~0x03;
							tempo = ntohl(tempo);
							//printf("Tempo: %X (%d) microseconds/quarter note\n", tempo, tempo);

							break;

						case 0x58:

							j += 5; // Skip time signature
							//printf("\n");

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


			//printf("(evt %d, ch %d) ", midi_event, midi_channel);

			u8 note;
			switch(midi_event)
			{
				case 8:
					note = *(buffer + j++);
					++j;
					//printf("%d up\n", note);
					break;
				case 9:
					note = *(buffer + j++);
					++j;
					
					if(note - DIFFICULTY_OFFSET >= 0 && note - DIFFICULTY_OFFSET < 5)
					{
						u8 channel = note - DIFFICULTY_OFFSET;

						note_init(&scene.channel[player][channel].note_list[note_index[channel]++], time/1000/time_div, TYPE_NORMAL);
						
						//printf("channel: %d, note: %d\n", channel, note_index[channel]-1);
					}
					//printf("%d down\n", note);
					break;
				default:

					printf("Unknown MIDI event\n", j);

					return -1;
			}
		}

		free(buffer);
	}

	fclose(file);

	return 0;
}


int FOF_load(char *folder_path)
{
	char song_path[128];

	strcpy(song_path, folder_path);
	
	u8 song_path_length = strlen(song_path);

	if(song_path[song_path_length - 1] != DIR_SEPARATOR)
	{
		song_path[song_path_length] = DIR_SEPARATOR;
		song_path[++song_path_length] = '\0';
	}

	printf("Song directory: %s\n", song_path);

	u8 i;

	for(i = 0; i < MUSIC_TRACKS; ++i)
	{
		strcat(song_path, music_file_names[i]);

		if(sound_load_ogg(song_path) != 0)
		{
			printf("Unable to load song.\n");
			return -1;
		}

		song_path[song_path_length] = '\0';
	}

	for(i = 0; i < 5; ++i)
	{
		scene.current_note[player][i] = 0;
	}

	load_mid(strcat(song_path, "notes.mid"));

	//scene.length = 1000000; // bs number
	
	scene.length = 0;

	for(i = 0; i < 5; ++i)
	{
		if(scene.channel[player][i].note_list[scene.channel[player][i].num_notes - 1].timestamp > scene.length)
			scene.length = scene.channel[player][i].note_list[scene.channel[player][i].num_notes - 1].timestamp;
	}
	
	scene.length += 12000; //Add a few sec for non-sudden shutdown
	
	
	return 0;
}

void FOF_close()
{
	u8 i;
	for(i = 0; i < 5; ++i)
	{
		channel_clear(&scene.channel[player][i]);
	}
}
