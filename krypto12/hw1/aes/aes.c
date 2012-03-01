#include "aes.h"

#include <string.h>
#include <stdint.h>

void rotate(unsigned char *in)
{
	unsigned char tmp = *in;
	int i;
	for(i = 0; i < 3; ++i)
	{
		*in = *(in+1);
		++in;
	}
	*in = tmp;
}

void schedule_core(unsigned char *in, unsigned char iter)
{
	// rotate input word
	rotate(in);
	int i;
	// apply sboxes
	for(i = 0; i < 4; ++i)
		in[i] = sbox[in[i]];

	// do rcon magic
	*in ^= rcon[iter];
}

void aes_key_expand_128(const unsigned char *key, unsigned char *out)
{
	unsigned char keys[11*16];
	memcpy(keys, key, 16*sizeof(char));

	unsigned char c = 16;
	unsigned char i = 1;
	unsigned char t[4];
	while(c < 11*16)
	{
		unsigned char j;
		for(j = 0; j < 4; ++j)
			t[j] = keys[c + (j-4)]; // copy last 4 bytes

		if(c % 16 == 0) {
			schedule_core(t, i);
			++i;
		}
		// copy back new key as old ^ t
		for(j = 0; j < 4; ++j) {
			keys[c] = keys[c-16] ^ t[j];
			++c;
		}
	}
	memcpy(out, keys, sizeof(char)*16*11);
}

void aes_encrypt_128(const unsigned char *key, FILE *in, FILE *out)
{
	unsigned char keys[11][16];
	aes_key_expand_128(key, &keys[0][0]);
	fwrite(keys, sizeof(char)*16, 11, stdout);
}

