#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdint.h>

int s_socket;

char welcome_msg[80+1] = "hax0r bank connected.";

typedef struct
{
	uint32_t user;
	uint16_t pin;

	float money;
} user;

user users[3] = {{12456789, 4711, 42},
		{101010, 10, 101.0},
		{1524324, 2034, 10234.0}};

int user_len = 3;

struct
{
	int socket;

	user *account;
} conn[42];
int connections = 0;

char buffer[10];

int main(int argc, char *argv[])
{
	int i;

	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(4711);
	addr.sin6_addr = in6addr_any;

	socklen_t addrlen = sizeof(addr);

	s_socket = socket(addr.sin6_family, SOCK_STREAM, 0);

	fcntl(0, F_SETFL, O_NONBLOCK);
	fcntl(s_socket, F_SETFL, O_NONBLOCK);

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

	while(1)
	{
		int foo = accept(s_socket, (struct sockaddr *) &addr, &addrlen);

		if(foo != -1)
		{
			printf("connected!\n");

			fcntl(foo, F_SETFL, O_NONBLOCK);

			conn[connections].socket = foo;
			conn[connections].account = NULL;
			connections++;
		}

		for(i = 0; i < connections; ++i)
		{
			int retval = recv(conn[i].socket, buffer, 1, 0);
			if(retval == 0)
			{
				close(conn[i].socket);
				conn[i--] = conn[--connections];
				continue;
			}
			else if(retval != -1)
			{
				uint32_t user;
				uint16_t pin;
				int j;
				float money;
				uint8_t code;
				switch(*buffer)
				{
					// login, sets account
					case 'l':
						recv(conn[i].socket, &user, sizeof(uint32_t), 0);
						user = ntohl(user);
						recv(conn[i].socket, &pin, sizeof(uint16_t), 0);
						pin = ntohs(pin);
						printf("login: %d:%d\n", user, pin);
						*buffer = 'e';
						// checks for user matching that ID
						for(j = 0; j < user_len; ++j)
						{
							if(users[j].user == user)
							{
								if(users[j].pin == pin)
								{
									conn[i].account = &users[j];
									*buffer = 'l';
								}
								break;
							}
						}
						send(conn[i].socket, buffer, 1, 0);
						break;
					// welcome message
					case 'm':
						send(conn[i].socket, welcome_msg, strlen(welcome_msg)+1, 0);
						break;
					// balance
					case 'b':
						if(conn[i].account == NULL)
						{
							*buffer = 'e';
							send(conn[i].socket, buffer, 1, 0);
							break;
						}
						*(float *)(buffer+1) = conn[i].account->money;
						send(conn[i].socket, buffer, 5, 0);
						break;
					// withdraw (subtract) money
					case 'w':
						recv(conn[i].socket, &code, 1, 0);
						recv(conn[i].socket, &money, sizeof(float), 0);

						if(conn[i].account == NULL || conn[i].account->money < money)
						{
							*buffer = 'e';
							send(conn[i].socket, buffer, 1, 0);
							break;
						}

						*buffer = 'w';
						*(float *)(buffer+1) = money;

						send(conn[i].socket, buffer, 5, 0);
						break;
					default:
						putchar(*buffer);
						printf("errorah! :(\n");
				}
			}
		}

		fgets(welcome_msg, 80+1, stdin);
	}

	if(close(s_socket) == -1)
	{
		perror("close");
		return -1;
	}

	//printf("close sockets.. :)");

	return 0;
}
