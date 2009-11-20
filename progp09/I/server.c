#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int s_socket;

#define conn_count 42
int conn[conn_count];
int next_conn = 0;

int main(int argc, char *argv[])
{
	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(4711);
	addr.sin6_addr = in6addr_any;

	socklen_t addrlen = sizeof(addr);

	s_socket = socket(addr.sin6_family, SOCK_STREAM, 0);

	if(s_socket == -1)
	{
		perror("socket");
		return -1;
	}

	if(bind(s_socket, (struct sockaddr *) &addr, addrlen) == -1)
	{
		perror("bind");
		return -1;
	}

	if(listen(s_socket, 42) == -1)
	{
		perror("listen");
		return -1;
	}

	int foo = accept(s_socket, (struct sockaddr *) &addr, &addrlen);

	if(foo == -1)
	{
		perror("accept");
		return -1;
	}

	printf("Someone connected!!! :D\n");

	// close foo? :)

	if(close(s_socket) == -1)
	{
		perror("close");
		return -1;
	}

	while(1);

	return 0;
}
