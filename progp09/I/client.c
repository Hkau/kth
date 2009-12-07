#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

// fixed-size types
#include <stdint.h>

struct
{
	char user[16+1];
	char pin[16+1];
	char shortcode[16+1];
	char balance[16+1];
	char withdraw[16+1];
	char exit[16+1];
} lang;

char welcome_msg[80+1]; // 80 + \0

char buffer[128];

int main(int argc, char *argv[])
{
	char *address, *port;

	printf("BANKINATOR BOOTUP SEQUENCE!\n");

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

	// select language

	strcpy(welcome_msg, "wellcome yo");
	strcpy(lang.user, "card number");
	strcpy(lang.pin, "pin code");
	strcpy(lang.shortcode, "single-use code");
	strcpy(lang.balance, "balance");
	strcpy(lang.withdraw, "withdraw");
	strcpy(lang.exit, "exit");

	if(connect(c_socket, addr->ai_addr, addr->ai_addrlen) != 0)
	{
		perror("connect");
		return -1;
	}

	while(1)
	{
		putchar('\n');

		*buffer = 'm';
		send(c_socket, buffer, 1, 0);
		recv(c_socket, welcome_msg, 80+1, 0);

		printf("%s\n", welcome_msg);

		printf("%s: ", lang.user);
		uint32_t user;
		scanf("%u", &user);

		uint16_t pin;
		printf("%s: ", lang.pin);
		scanf("%hu", &pin);

		*buffer = 'l';
		*(uint32_t *)(buffer+1) = htonl(user);
		*(uint16_t *)(buffer+5) = htons(pin);

		send(c_socket, buffer, 7, 0);
		if(recv(c_socket, buffer, 1, MSG_WAITALL) == -1)
		{
			printf("fail :(\n");
			perror("recv");
			return -1;
		}
		if(*buffer != 'l')
		{
			printf("login failed\n");
			continue;
		}
		while(1)
		{
			printf("(b) %s (w) %s (e) %s\n", lang.balance, lang.withdraw, lang.exit);
			scanf("%s", buffer);
			if(buffer[1] != '\0')
				continue;
			if(*buffer == 'e')
				break;

			switch(*buffer)
			{
				case 'b':
					send(c_socket, buffer, 1, 0);
					recv(c_socket, buffer, 5, 0);
					if(*buffer != 'b')
					{
						printf("protocol error, unable to retrieve balance.\n");
						return -1;
					}
					printf("%s: %f\n", lang.balance, *(float *)(buffer+1));
					break;
				case 'w':
					printf("no, I like hot tamale\n");
					break;
				default:
					break;
			}
		}
	}

	freeaddrinfo(addr);

	if(close(c_socket) == -1)
	{
		perror("close");
		return -1;
	}


	return 0;
}
