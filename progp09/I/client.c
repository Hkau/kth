#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	char *address, *port;

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

	printf("finito\n");

	return 0;
}
