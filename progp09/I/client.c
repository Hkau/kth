#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

// fixed-size types
#include <stdint.h>

struct
{
	char welcome_msg[80+1]; // 80 + \0
	char username_msg[16+1];
	char pin_msg[16+1];
	char shortcode_msg[16+1];
} lang;

int main(int argc, char *argv[])
{
	char *address, *port;

	printf("BANKINATOR BOOTUP SEQUENCE!\n\n\n");

	if(argc > 1)
		address = argv[1];
	else
		address = "localhost";

	if(argc > 2)
		port = argv[2];
	else
		port = "4711";

	struct addrinfo *addr, hints;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int err = getaddrinfo(address, port, &hints, &addr);

	if(err != 0)
	{
		fprintf(stderr, "lookup: %s", gai_strerror(err));
		return err;
	}

	int c_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if(c_socket == -1)
	{
		perror("socket");
		return -1;
	}

	if(connect(c_socket, addr->ai_addr, addr->ai_addrlen) != 0)
	{
		perror("connect");
		return -1;
	}

	// select language

	strcpy(lang.welcome_msg, "wellcome yo");
	strcpy(lang.username_msg, "username");
	strcpy(lang.pin_msg, "pin code");
	strcpy(lang.shortcode_msg, "single-use code");

	while(1)
	{
		printf("%s\n", lang.welcome_msg);

		printf("%s: ", lang.username_msg);
		uint32_t username;
		scanf("%u", &username);

		uint16_t pin;
		printf("%s: ", lang.pin_msg);
		scanf("%hu", &pin);
	}

	freeaddrinfo(addr);

	return 0;
}
