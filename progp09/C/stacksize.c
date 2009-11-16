#include <stdio.h>

int main()
{
	long long ll1, ll2;
	unsigned int i1, i2;
	char foo[5];
	float f;

	printf("rubrikraden\n");
	printf("ll1 - size: %u, value: %lld, address: 0x%X\n", sizeof(ll1), ll1, (unsigned int)&ll1);
	printf("ll2 - size: %u, value: %lld, address: 0x%X\n", sizeof(ll2), ll2, (unsigned int)&ll2);
	printf("i1 - size: %u, value: %u, address: 0x%X\n", sizeof(i1), i1, (unsigned int)&i1);
	printf("i2 - size: %u, value: %u, address: 0x%X\n", sizeof(i2), i2, (unsigned int)&i2);
	// tolkar foo:s värde som variabelns värde = adressen
	printf("foo - size: %u, value: %u, address: 0x%X\n", sizeof(foo), (unsigned int)foo, (unsigned int)&foo);
	printf("f - size: %u, value: %f, address: 0x%X\n", sizeof(f), f, (unsigned int)&f);

	return 0;
}
