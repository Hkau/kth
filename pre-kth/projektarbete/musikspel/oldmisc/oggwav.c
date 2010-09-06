#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"


int main(int argc, char *argv[])
{
	printf("\nOgg to WAV decoder/converter.\n\n");
	if(argc > 2)
	{
		printf("No input file.\n\n");
		
		return 1;
	}

	OggVorbis_File vf;

	if(ov_fopen(argv[1], &vf) < 0)
	{
		printf("Unable to open file: \"%s\".\n\n");
		return 1;
	}
	
	vorbis_info *vi = ov_info(&vf, -1);

	printf("Vorbis file info:\n\n");
	printf("Channels: %d\n", vi->channels);
	printf("Samples: %ld\n", ov_pcm_total(&vf, -1));
	printf("Samplerate: %d.\n", vi->rate);
	printf("\nLength: ~%ld s.\n", ov_pcm_total(&vf, -1)/vi->rate);


	uint32_t pcm_size = ov_pcm_total(&vf, -1) * vi->channels * 2;
	printf("\nPCM size: %ld bytes.\n", pcm_size);
	printf("\nWAV size: %ld bytes.\n", pcm_size+44);

	char buffer[4096];

	FILE *out = fopen("out.wav", "wb");

	if(out == NULL)
	{
		printf("Unable to open outfile for writing.\n\n");
		return 1;
	}

	uint32_t temp32;
	uint16_t temp16;

	// Chunk header
	memcpy(buffer, "RIFF", 4);
	temp32 = 36+pcm_size;
	memcpy(&temp32, &buffer[4], 4);
	memcpy(&buffer[8], "WAVE", 4);

	// SubChunk1
	memcpy(&buffer[12], "fmt ", 4);
	temp32 = 16;
	memcpy(&buffer[16], &temp32, 4);
	temp16 = 1;
	memcpy(&buffer[20], &temp16, 2);
	temp16 = vi->channels;
	memcpy(&buffer[22], &temp16, 2);
	temp32 = vi->rate;
	memcpy(&buffer[24], &temp32, 4);
	temp32 = vi->rate * vi->channels * 2;
	memcpy(&buffer[28], &temp32, 4);
	temp16 = vi->channels * 2;
	memcpy(&buffer[32], &temp16, 2);
	temp16 = 16;
	memcpy(&buffer[34], &temp16, 2);
	
	// SubChunk2
	memcpy(&buffer[36], "data", 4);
	memcpy(&buffer[40], &pcm_size, 4);
	
	// Write WAV header
	fwrite(buffer, 1, 44, out);

	printf("\nDecoding:\n\n");
	int current_section;

	while(1)
	{
		long ret = ov_read(&vf, buffer,sizeof(buffer), 0, 2, 1, &current_section);
		if(ret == 0)
			break;

		if(ret < 0)
		{
			printf("Error while reading.\n");
			break;
		}
		
		fwrite(buffer, 1, ret, out);
	}

	printf("Done.\n");
	fclose(out);	

	ov_clear(&vf);

	return 0;
}
