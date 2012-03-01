#ifndef AES_H
#define AES_H

#include <stdio.h>

void aes_encrypt_128(const unsigned char *key, FILE *in, FILE *out);

// As rcon and sbox both map bytes on bytes, we can afford to store
// a lookup table.
extern unsigned char sbox[256];
extern unsigned char rcon[256];

#endif

