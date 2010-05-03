#include "sound.h"

#include "SDL.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include <stdio.h>

#include "types.h"

#define OGG_CHANNELS 8

OggVorbis_File vf[OGG_CHANNELS];

bool mute[OGG_CHANNELS];
bool open[OGG_CHANNELS];

bool audio_open = false;

u8 channels = 0;
int current_section;

u8 *buffer; //pcm buffer for mixing

void callback(void *userdata, Uint8 *stream, int len)
{
	long stream_filled_size = 0;

	u8 i;

	for(i = 0; i < OGG_CHANNELS; ++i)
	{
		if(open[i])
		{
			while(stream_filled_size < len)
			{
				long read_size = ov_read(&vf[0], stream + stream_filled_size, len - stream_filled_size, 0, 2, 1, &current_section);
		
				if(read_size <= 0)
				{
					if(read_size == 0)
						break;
			
					//SDL_PauseAudio(1);
					mute[i] = true;
					open[i] = false;
					ov_clear(&vf[i]);
			
					return;
				}
		
				stream_filled_size += read_size;
			}

			++i;

			break;
		}
	}

	for(;i < OGG_CHANNELS; ++i)
	{
		if(open[i])
		{
			stream_filled_size = 0;

			while(stream_filled_size < len)
			{
				long read_size = ov_read(&vf[i], buffer + stream_filled_size, len - stream_filled_size, 0, 2, 1, &current_section);
		
				if(read_size <= 0)
				{
					if(read_size == 0)
						break;

					mute[i] = true;
					open[i] = false;
					ov_clear(&vf[i]);
			
					return;
				}
		
				stream_filled_size += read_size;
			}
			SDL_MixAudio(stream, buffer, len, SDL_MIX_MAXVOLUME);
		}
	}

	if(stream_filled_size < len)
	{
		memset (stream + stream_filled_size, 0, len - stream_filled_size);
	}
}

int sound_load_ogg(char *path)
{
	u8 i;
	for(i = 0; i < OGG_CHANNELS; ++i)
	{
		if(open[i] == false)
			break;
	}

	if(i == OGG_CHANNELS)
		return -1;
		
	printf("%s\n", path);

	int result = ov_fopen(path, &vf[i]);

	if(result < 0)
	{
		printf("Unable to open file: \"%s\".\n\n", path);
		return result;
	}

	vorbis_info *vi = ov_info(&vf[i], -1);

	SDL_AudioSpec fmt;

	fmt.freq = vi->rate;
	fmt.format = AUDIO_S16;
	fmt.channels = vi->channels;
	fmt.samples = 2048;
	fmt.callback = &callback;
	fmt.userdata = NULL;

	if(!audio_open)
	{
		buffer = malloc(fmt.samples*2*vi->channels); // samples per channel and buffer, 2 bytes/sample (S16), channels
		if(SDL_OpenAudio(&fmt, NULL) < 0)
		{
			printf("Unable to open audio output.\n\n");
			return -1;
		}
		audio_open = true;
	}
	
	mute[i] = false;
	open[i] = true;
	
	return 0;
}

int sound_play()
{
	SDL_PauseAudio(0);

	return 0;
}

int sound_init()
{
	u8 i;
	for(i = 0; i < OGG_CHANNELS; ++i)
	{
		open[i] = false;
		mute[i] = true;
	}

	return SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void sound_unload_all()
{
	u8 i;
	for(i = 0; i < OGG_CHANNELS; ++i)
	{
		if(open[i])
		{
			ov_clear(&vf[i]);
			open[i] = false;
		}
	}
}
void sound_quit()
{
	sound_unload_all();

	if(audio_open)
		free(buffer);

	audio_open = false;

	SDL_CloseAudio();

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

