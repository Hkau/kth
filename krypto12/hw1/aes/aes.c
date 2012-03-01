#include "aes.h"

#include <string.h>
#include <stdint.h>

// Rotate 4 bytes one step
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

// Rotate, apply sbox and 2^i exponentiation in GF(2^8) using lookup table.
void schedule_core(unsigned char *in, unsigned char iter)
{
	// rotate input word
	rotate(in);
	int i;

	// apply sboxes
	for(i = 0; i < 4; ++i)
		in[i] = sbox[in[i]];

	// xor first byte with 2^iter in GF(2^8)
	*in ^= rcon[iter];
}

// key expansion schedule
void aes_key_expand_128(const unsigned char *key, unsigned char *keys)
{
	// first 16 bytes are simply the key
	memcpy(keys, key, 16*sizeof(char));

	unsigned char c = 16; // copied char count
	unsigned char i = 1; // working on ith subkey
	unsigned char t[4]; // temporary 32-bit block

	while(c < 11*16) // while not done with our 11 subkeys
	{
		unsigned char j;
		for(j = 0; j < 4; ++j)
			t[j] = keys[c + (j-4)]; // copy last 4 bytes

		// perform core schedule only for first block of each key
		if(c % 16 == 0) {
			schedule_core(t, i);
			++i;
		}

		// new key bits are old key bits ^ new column
		for(j = 0; j < 4; ++j)
		{
			keys[c] = keys[c-16] ^ t[j];
			++c;
		}
	}
}

// expanded subkeys
unsigned char keys[11][16];

// Simply xor the state with the round key
void AddRoundKey(unsigned char *buffer, unsigned char round)
{
	int i;
	for(i = 0; i < 16; ++i)
		buffer[i] ^= keys[round][i];
}

// Apply S-box
void SubBytes(unsigned char *buffer)
{
	int i;
	for(i = 0; i < 16; ++i)
		buffer[i] = sbox[buffer[i]];
}

// Hand-written row-shifting.
void ShiftRows(unsigned char *buffer)
{
	unsigned char tmp;

	// first row, shift one column left
	tmp = buffer[1];
	buffer[1] = buffer[5];
	buffer[5] = buffer[9];
	buffer[9] = buffer[13];
	buffer[13] = tmp;

	// second row, shift two columns left
	tmp = buffer[2];
	buffer[2] = buffer[10];
	buffer[10] = tmp;
	tmp = buffer[6];
	buffer[6] = buffer[14];
	buffer[14] = tmp;

	// third row, shift three columns left = one column right
	tmp = buffer[15];
	buffer[15] = buffer[11];
	buffer[11] = buffer[7];
	buffer[7] = buffer[3];
	buffer[3] = tmp;
}

// Fast column mixer.
//
// Explained at: http://www.samiam.org/mix-column.html
//
// Important points:
// a[n] = gmul(r[n], 1)
// b[n] = gmul(r[n], 2)
// gmul(r[n],3) = a[n]^b[n]
// - b[n] is can be computed with a shift, and conditional xor depending on
//   r[n]'s high bit.
// - Addition is ^ within Rijndael's finite field.
void gmix_column(unsigned char *r) {
	unsigned char a[4];
	unsigned char b[4];
	unsigned char c;

	// Copy r[n] to a[n] and calculate b[n]
	for(c=0;c<4;c++) {
		a[c] = r[c];
		b[c] = r[c] << 1;
		if(r[c] & 0x80) // if r[c]'s high bit is set
			b[c] ^= 0x1B; // xor with 0x1B
	}

	// Apply matrix multiplication to the r[] vector within GF(2^8)
	r[0] = b[0] ^ a[3] ^ a[2] ^ (b[1] ^ a[1]); /* 2 * a0 + a3 + a2 + 3 * a1 */
	r[1] = b[1] ^ a[0] ^ a[3] ^ (b[2] ^ a[2]); /* 2 * a1 + a0 + a3 + 3 * a2 */
	r[2] = b[2] ^ a[1] ^ a[0] ^ (b[3] ^ a[3]); /* 2 * a2 + a1 + a0 + 3 * a3 */
	r[3] = b[3] ^ a[2] ^ a[1] ^ (b[0] ^ a[0]); /* 2 * a3 + a2 + a1 + 3 * a0 */
}

// Apply column mixing individually to each column
void MixColumns(unsigned char *buffer)
{
	int i;
	for(i = 0; i < 4; ++i)
		gmix_column(&buffer[4*i]);
}

// Perform aes on a single 16-byte block
void aes_block_128(unsigned char *buffer)
{
	AddRoundKey(buffer, 0);
	int i;
	for(i = 1; i < 10; ++i)
	{
//		fwrite(buffer, sizeof(char), 16, stdout);
		SubBytes(buffer);
//		fwrite(buffer, sizeof(char), 16, stdout);
		ShiftRows(buffer);
//		fwrite(buffer, sizeof(char), 16, stdout);
		MixColumns(buffer);
//		fwrite(buffer, sizeof(char), 16, stdout);
		AddRoundKey(buffer, i);
	}
	SubBytes(buffer);
	ShiftRows(buffer);
	AddRoundKey(buffer, 10);
}

// ECB mode of AES-128
void aes_encrypt_128(const unsigned char *key, FILE *in, FILE *out)
{
	aes_key_expand_128(key, &keys[0][0]);
//	fwrite(keys, sizeof(char)*16, 11, stdout); // print key schedule

	unsigned char buffer[16];
	while(fread(buffer, sizeof(char), 16, in))
	{
		aes_block_128(buffer);
		fwrite(buffer, sizeof(char), 16, out); // Print out each block (ECB)
	}
}

