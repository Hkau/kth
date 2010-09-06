#include "SDL.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include <stdbool.h>

OggVorbis_File vf;
int current_section;
bool quit = false;

void callback(void *userdata, Uint8 *stream, int len)
{
	long stream_filled_size = 0;

	while(stream_filled_size < len)
	{
		long read_size = ov_read(&vf, stream + stream_filled_size, len - stream_filled_size, 0, 2, 1, &current_section);
		
		if(read_size <= 0)
		{
			quit = true;

			if(read_size == 0)
				break;
			
			SDL_PauseAudio(1);
			
			return;
		}
		
		stream_filled_size += read_size;
	}

	if(stream_filled_size < len)
	{
		memset (stream + stream_filled_size, 0, len - stream_filled_size);
	}
}


int main (int argc, char *argv[])
{
	printf("\nCrappy ogg player\n\n");

	if(argc < 2)
	{
		printf("No input file.\n\n");
		
		return 1;
	}

	if(ov_fopen(argv[1], &vf) < 0)
	{
		printf("Unable to open file: \"%s\".\n\n", argv[1]);
		return 1;
	}
	
	vorbis_info *vi = ov_info(&vf, -1);

	printf("Vorbis file info:\n\n");
	printf("Channels: %d\n", vi->channels);
	printf("Samplerate: %d.\n", vi->rate);
	printf("\nLength: ~%ld s.\n", ov_pcm_total(&vf, -1)/vi->rate);

	SDL_Init(SDL_INIT_AUDIO);

	SDL_AudioSpec fmt;

	fmt.freq = vi->rate;
	fmt.format = AUDIO_S16;
	fmt.channels = vi->channels;
	fmt.samples = 1024;
	fmt.callback = &callback;
	fmt.userdata = NULL;
	
	if(SDL_OpenAudio(&fmt, NULL) < 0)
	{
		printf("Unable to open audio output.\n\n");
		
		return -1;
	}

	printf("\nPlaying...");

	SDL_PauseAudio(0);

	while(!quit)
	{
		static SDL_Event event;
		while(SDL_PollEvent(&event));

		SDL_Delay(100);
	}

	printf("Done.\n");
	
	SDL_CloseAudio();

	ov_clear(&vf);

	SDL_Quit();

	return 0;
}

