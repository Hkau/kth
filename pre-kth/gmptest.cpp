#include "gmp.h"
#include <cstdio>

int main()
{
	mpz_t integer;
	
	mpz_init_set_str(integer, "2", 10);

	mpz_t int2;
	
	mpz_init_set_str(int2, "2", 10);

	for(unsigned int i = 0; i < 31; ++i)
	{
		printf("lol: %d\n", i);
		//mpz_mul(integer, integer, int2);
	}
	
	mpz_out_str(stdout, 10, integer);
	
	printf("\n");

	mpz_clear(integer);

	return 0;
}
