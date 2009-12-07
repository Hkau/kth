#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

// fixed-size types
#include <stdint.h>

// current language strings
struct
{
	char name[2];

	char user[16+1];
	char pin[16+1];
	char shortcode[16+1];
	char balance[16+1];
	char withdraw[16+1];
	char exit[16+1];
} lang;

// 80-char welcome message as specified
char welcome_msg[80+1]; // 80 + \0

// buffer used for sending/recieving data
char buffer[128];

int main(int argc, char *argv[])
{
	char *address, *port;

	printf("BANKINATOR BOOTUP SEQUENCE!\n");

	// read address/port arguments if existing
	if(argc > 1)
		address = argv[1];
	else
		address = "localhost";

	if(argc > 2)
		port = argv[2];
	else
		port = "4711";

	// addresses, for converting string to address
	struct addrinfo *addr, hints;

	memset(&hints, 0, sizeof(hints));

	// may be ipv6 or ipv4, both work
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// resolve address
	int err = getaddrinfo(address, port, &hints, &addr);

	// print error and quit if one occured
	if(err != 0)
	{
		fprintf(stderr, "lookup: %s", gai_strerror(err));
		return err;
	}

	// create client socket
	int c_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

	if(c_socket == -1)
	{
		perror("socket");
		return -1;
	}

	// connect to server
	if(connect(c_socket, addr->ai_addr, addr->ai_addrlen) != 0)
	{
		perror("connect");
		return -1;
	}

	// set language name to something invalid so that it'll update
	lang.name[0] = 0;

	while(1)
	{
		putchar('\n');

		// request welcome message
		*buffer = 'm';
		send(c_socket, buffer, 1, 0);
		recv(c_socket, welcome_msg, 80+1, 0);

		printf("%s\n", welcome_msg);

		// recieve and print language list
		printf("LANGUAGE: ");
		*buffer = '?';
		send(c_socket, buffer, 1, 0);
		recv(c_socket, buffer, sizeof(buffer), 0);

		// print out all recieved language codes
		int i;
		for(i = 0; i < buffer[1]; ++i)
		{
			putchar('\n');
			putchar(buffer[2+2*i]);
			putchar(buffer[2+2*i+1]);
		}

		// select language
		printf("\nchoose language: ");

		char langname[4];
		scanf("%s", langname);

		if(langname[2] != '\0')
			continue;

		// download if language isn't the current one
		if(lang.name[0] != langname[0] || lang.name[1] != langname[1])
		{
			// send language request

			*buffer = 'x';
			buffer[1] = langname[0];
			buffer[2] = langname[1];
			send(c_socket, buffer, 3, 0);

			// check error/message code
			recv(c_socket, buffer, 1, 0);
			if(*buffer != 'x')
				continue;

			// OK! read rest of message
			recv(c_socket, buffer, sizeof(buffer), 0);
			lang.name[0] = langname[0];
			lang.name[1] = langname[1];

			// ugly thing copies back from buffer to languages each 16+'\0' string
			int k = 0;
			memcpy(lang.user, buffer+k, 16+1);
			k += 16+1;
			memcpy(lang.pin, buffer+k, 16+1);
			k += 16+1;
			memcpy(lang.shortcode, buffer+k, 16+1);
			k += 16+1;
			memcpy(lang.balance, buffer+k, 16+1);
			k += 16+1;
			memcpy(lang.withdraw, buffer+k, 16+1);
			k += 16+1;
			memcpy(lang.exit, buffer+k, 16+1);
		}

		// login sequence
		printf("\n%s: ", lang.user);
		uint32_t user;
		scanf("%u", &user);

		uint16_t pin;
		printf("%s: ", lang.pin);
		scanf("%hu", &pin);

		// htonl/ntohl is important for converting to network endianism
		*buffer = 'l';
		*(uint32_t *)(buffer+1) = htonl(user);
		*(uint16_t *)(buffer+5) = htons(pin);

		// send login request
		send(c_socket, buffer, 7, 0);
		if(recv(c_socket, buffer, 1, 0) == -1)
		{
			printf("fail :(\n");
			perror("recv");
			return -1;
		}
		// restart if login failed
		if(*buffer != 'l')
		{
			printf("login failed\n");
			continue;
		}
		// login ok, proceed to check out the account
		while(1)
		{
			// menu, awesome thing stuff
			printf("(b) %s (w) %s (e) %s\n", lang.balance, lang.withdraw, lang.exit);
			scanf("%s", buffer);

			if(buffer[1] != '\0')
				continue;
			if(*buffer == 'e')
				break;

			float money;
			short code;

			// check menu choice (first character)
			switch(*buffer)
			{
				// check account balance
				case 'b':
					send(c_socket, buffer, 1, 0);
					recv(c_socket, buffer, 5, 0);
					if(*buffer != 'b')
					{
						printf("balance error\n");
						return -1;
					}
					printf("%s: %f\n", lang.balance, *(float *)(buffer+1));
					break;
				// withdraw money from account
				case 'w':
					// ask for short single-use code
					printf("%s: ", lang.shortcode);
					scanf("%hu", &code);

					// ask for amount to withdraw
					printf("%s: ", lang.withdraw);
					scanf("%f", &money);

					// strip single-use code down to 1 byte
					*(uint8_t *)(buffer+1) = code & 0xFF;
					*(float *)(buffer+2) = money;

					// send withdraw request
					send(c_socket, buffer, 6, 0);
					recv(c_socket, buffer, 5, 0);

					// say that an error occured if withdraw request didn't fall through
					if(*buffer != 'w')
						printf("error");
					break;
				// invalid option. :)
				default:
					break;
			}
		}
	}

	// good stuff to do, unfortunately the client won't reach this point, foreverloopstyle.
	freeaddrinfo(addr);

	if(close(c_socket) == -1)
	{
		perror("close");
		return -1;
	}


	return 0;
}
