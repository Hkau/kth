#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint8_t charval(char c)
{
	switch(c)
	{
		case 'ä':
			return 27;
		case 'å':
			return 28;
		case 'ö':
			return 29;
		default:
			return c - 'a' + 1;
	}
}

uint32_t lazypos(char *c)
{
	uint32_t val = 30*30*(charval(*c)-1);

	if(c[1] == '\0')
		return val;

	val += 30 * charval(c[1]);

	if(c[2] == '\0')
		return val;
	
	val += charval(c[2]);

	return val;
}

uint32_t errpos = -1;

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("usage: konkordans <word> | konkordans -c\n");
		return -1;
	}
	if(argc >= 2 && strcmp(argv[1], "-c") == 0)
	{
		FILE *pos_idx = fopen("pos_idx", "w");
		FILE *word_idx = fopen("word_idx", "w");
		FILE *prefix_idx = fopen("prefix_idx", "w");

		char wordbuffer[128] = "";
		char oldword[128] = "";

		uint32_t prefix_pos = 0;
		uint32_t word_pos = 0, word_last_pos = 0;
		uint32_t pos = 0, last_pos = 0;
		uint32_t file_pos = 0, pos_count = 0;

		if(pos_idx == NULL || word_idx == NULL || prefix_idx == NULL)
		{
			fprintf(stderr, "shit just got real (unable to open file)s\n");
		}

		while(!feof(stdin))
		{
			if(scanf("%s %d", wordbuffer, &file_pos) > 1)
			{
				if(strcmp(wordbuffer, oldword) != 0) // new word!
				{
					if(pos != 0)
					{
						fwrite(&last_pos, 1, sizeof(last_pos), word_idx);
						last_pos = pos;
						word_pos += sizeof(last_pos);
						fwrite(&pos_count, 1, sizeof(pos_count), word_idx);
						pos_count = 0;
						word_pos += sizeof(pos_count);
					}

					// find new word stem file pos
					uint32_t newpos = lazypos(wordbuffer);

					// fill word_idx prefix file
					if(newpos != prefix_pos)
					{
						//printf("prefix::%s:%s  %d::%d :word_pos: %d\n", oldword, wordbuffer, prefix_pos, newpos, word_pos);

						// write new word as start point for stem.
						fwrite(&word_last_pos, 1, sizeof(word_last_pos), prefix_idx);
						word_last_pos = word_pos;
						++prefix_pos;

						while(prefix_pos < newpos)
						{
							fwrite(&errpos, 1, sizeof(errpos), prefix_idx);
							++prefix_pos;
						}
					}

					word_pos += fprintf(word_idx, "%s", wordbuffer);

					fputc('\0', word_idx);
					word_pos++;

					strcpy(oldword, wordbuffer);
				}

				fwrite(&file_pos, 1, sizeof(file_pos), pos_idx);
				pos++;
				pos_count++;
			}
			else
			{
				// last thing before feof! :o
				fwrite(&last_pos, 1, sizeof(last_pos), word_idx);
				fwrite(&pos_count, 1, sizeof(pos_count), word_idx);
			}
		}

		fclose(pos_idx);
		fclose(prefix_idx);
		fclose(word_idx);

		return 0;
	}

	// seek and destroy!

	FILE *prefix_idx = fopen("prefix_idx", "r");

	uint32_t pos = lazypos(argv[1]);

	fseek(prefix_idx, pos * sizeof(uint32_t), SEEK_SET);

	uint32_t offset;

	fread(&offset, 1, sizeof(offset), prefix_idx);
	fclose(prefix_idx);

	if(offset == errpos)
		printf("Det finns inga förekomster av ordet i filen! :(\n");

	// OFFSET HITTAD! <3 Nu ska vi leta lite ord!

	FILE *word_idx = fopen("word_idx", "r");

	fseek(word_idx, offset, SEEK_SET);

	char buffer[128];
	int bufind = 0;

	int cmp;
	uint32_t pos_start;
	uint32_t count;

	while(!feof(word_idx))
	{
		do
		{
			buffer[bufind++] = fgetc(word_idx);
		} while(buffer[bufind-1] != '\0' && !feof(word_idx));

		cmp = strcmp(argv[1], buffer);

		bufind = 0;

		fread(&pos_start, sizeof(pos_start), 1, word_idx);

		fread(&count, sizeof(count), 1, word_idx);

		if(cmp <= 0)
			break;
	}

	fclose(word_idx);

	if(cmp == 0)
	{
		printf("Det finns %d förekomster av ordet!\n", count);

		FILE *pos_idx = fopen("pos_idx", "r");
		FILE *korpus = fopen("korpus", "r");

		fseek(pos_idx, pos_start*sizeof(pos_start), SEEK_SET);

		int len = strlen(argv[1]);

		while(count-- != 0)
		{
			uint32_t pos;
			fread(&pos, 1, sizeof(pos), pos_idx);

			int fill_len = 60;
			if(argc > 2)
				sscanf(argv[2], "%d", &fill_len);

			fseek(korpus, (pos-fill_len/2 < 0 ? 0 : pos-fill_len/2), SEEK_SET);


			int read = fread(buffer, 1, fill_len+len, korpus);

			// centers output in special case (start of file)
			while(pos++ < fill_len/2)
			{
				putchar(' ');
				read --;
			}

			buffer[read] = '\0';

			char *c;
			for( c = buffer; *c != '\0'; ++c)
			{
				if(*c == '\n' || *c == '\t')
					putchar(' ');
				else
					putchar(*c);
			}
			putchar('\n');
		}
	}
	else
		printf("Det finns 0 förekomster av ordet!\n");


	return 0;
}

