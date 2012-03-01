#include "aes.h"
#include <stdio.h>

int main()
{
	unsigned char key[16];
	fread(key, sizeof(unsigned char), 16, stdin);
	
	aes_encrypt_128(key, stdin, stdout);

	return 0;
}
