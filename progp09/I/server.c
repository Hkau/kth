#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdint.h>

// server listening socket
int s_socket;

// welcome message with default message, can be changed later
char welcome_msg[80+1] = "hax0r bank connected.";

// user representation
typedef struct
{
	uint32_t user;
	uint16_t pin;

	float money;
} user;

// the bank's users
user users[3] = {{12456789, 4711, 42},
		{101010, 10, 101.0},
		{1524324, 2034, 10234.0}};

// number of users (length of list)
int user_len = 3;

// connections
struct
{
	int socket;

	user *account;
} conn[42];

// existing connections
int connections = 0;

// buffer used for sending packets
char buffer[10];

// languages contain these strings (and a name)
typedef struct
{
	char name[2];

	char user[16+1];
	char pin[16+1];
	char shortcode[16+1];
	char balance[16+1];
	char withdraw[16+1];
	char exit[16+1];
} lang;

// languages, english and swedish
lang langs[2] = {{{'e','n'}, "card number", "pin code", "single-use code", "balance", "withdraw", "exit"},
		{{'s','v'}, "kortnummer", "pin-kod", "engångskod", "saldo", "utdrag", "avsluta"}};

// number of languages
char lang_len = 2;

int main(int argc, char *argv[])
{
	int i;

	// create a socket address
	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));

	// ipv6 socket, localhost, 4711
	addr.sin6_family = AF_INET6;
	addr.sin6_port = htons(4711);
	addr.sin6_addr = in6addr_any;

	socklen_t addrlen = sizeof(addr);

	// create socket
	s_socket = socket(addr.sin6_family, SOCK_STREAM, 0);

	// if socket couldn't be made, print error
	if(s_socket == -1)
	{
		perror("socket");
		return -1;
	}

	// set socket and stdin to be non-blocking, so that we can handle multiple inputs
	fcntl(0, F_SETFL, O_NONBLOCK);
	fcntl(s_socket, F_SETFL, O_NONBLOCK);

	// bind socket to address
	if(bind(s_socket, (struct sockaddr *) &addr, addrlen) == -1)
	{
		perror("bind");
		return -1;
	}

	// start listening for connections (42 max)
	if(listen(s_socket, 42) == -1)
	{
		perror("listen");
		return -1;
	}

	while(1)
	{
		// try accepting at socket
		int foo = accept(s_socket, (struct sockaddr *) &addr, &addrlen);

		// if someone new connected
		if(foo != -1)
		{
			printf("connected!\n");

			// set this new socket to be non-blocking
			fcntl(foo, F_SETFL, O_NONBLOCK);

			// create a connection struct for this client
			conn[connections].socket = foo;
			conn[connections].account = NULL;
			connections++;
		}

		// handle all connections' 
		for(i = 0; i < connections; ++i)
		{
			int retval = recv(conn[i].socket, buffer, 1, 0);
			// client exited
			if(retval == 0)
			{

				close(conn[i].socket);
				conn[i--] = conn[--connections];
				continue;
			}
			// client has sent a message
			else if(retval != -1)
			{
				uint32_t user;
				uint16_t pin;
				int j, k;
				float money;
				uint8_t code;
				char langname[2];

				// what kind of message?
				switch(*buffer)
				{
					// language listing
					case '?':
						buffer[1] = lang_len;
						for(j = 0; j < lang_len; ++j)
						{
							// write language names into buffer
							buffer[2+2*j] = langs[j].name[0];
							buffer[2+2*j+1] = langs[j].name[1];
						}
						send(conn[i].socket, buffer, 2+2*lang_len, 0);
						break;
					// send language
					case 'x':
						recv(conn[i].socket, &langname, 2, 0);
						*buffer = 'e';
						for(j = 0; j < lang_len; ++j)
						{
							// if the language searched for exists
							if(langs[j].name[0] != langname[0] || langs[j].name[1] != langname[1])
								continue;
							// we have a valid language (reply isn't 'e' anymore)
							*buffer = 'x';
							// aaand copy all strings into the buffer
							k = 1;
							memcpy(buffer+k, langs[j].user, 16+1);
							k+=16+1;
							memcpy(buffer+k, langs[j].pin, 16+1);
							k+=16+1;
							memcpy(buffer+k, langs[j].shortcode, 16+1);
							k+=16+1;
							memcpy(buffer+k, langs[j].balance, 16+1);
							k+=16+1;
							memcpy(buffer+k, langs[j].withdraw, 16+1);
							k+=16+1;
							memcpy(buffer+k, langs[j].exit, 16+1);
							k+=16+1;
						}
						// send reply (error or all strings)
						send(conn[i].socket, buffer, k, 0);
						break;
					// login, sets account
					case 'l':
						// read user name
						recv(conn[i].socket, &user, sizeof(uint32_t), 0);
						user = ntohl(user);
						// read user pass
						recv(conn[i].socket, &pin, sizeof(uint16_t), 0);
						pin = ntohs(pin);
						// log login in plaintext, for that security extravaganza
						printf("login: %d:%d\n", user, pin);
						*buffer = 'e';
						// checks for user matching that ID
						for(j = 0; j < user_len; ++j)
						{
							if(users[j].user == user)
							{
								if(users[j].pin == pin)
								{
									// valid login, associate connection with account
									conn[i].account = &users[j];
									*buffer = 'l';
								}
								break;
							}
						}
						// send reply (error or login (e/l))
						send(conn[i].socket, buffer, 1, 0);
						break;
					// welcome message
					case 'm':
						// simply send the stored welcome message
						send(conn[i].socket, welcome_msg, strlen(welcome_msg)+1, 0);
						break;
					// balance
					case 'b':
						// make sure that the account is logged on
						if(conn[i].account == NULL)
						{
							*buffer = 'e';
							send(conn[i].socket, buffer, 1, 0);
							break;
						}
						// send back status
						*(float *)(buffer+1) = conn[i].account->money;
						send(conn[i].socket, buffer, 5, 0);
						break;
					// withdraw (subtract) money
					case 'w':
						// recieve single-digit code
						recv(conn[i].socket, &code, 1, 0);
						// money to be withdrawn
						recv(conn[i].socket, &money, sizeof(float), 0);

						// invalid if user isn't logged in, doesn't have enough money, or single-use code is invalid
						if(conn[i].account == NULL || conn[i].account->money < money || code % 2 != 1)
						{
							*buffer = 'e';
							send(conn[i].socket, buffer, 1, 0);
							break;
						}

						// send back reply confirming removed amount just to be sure.
						*(float *)(buffer+1) = money;

						// send reply and remove moneys
						send(conn[i].socket, buffer, 5, 0);
						conn[i].account->money -= money;
						break;
					default:
						// invalid packet
						printf("errorah! :(\n");
				}
			}
		}

		// update welcome message if one has been entered, this doesn't block and won't replace unless one has been entered
		fgets(welcome_msg, 80+1, stdin);
	}

	// this would normally close the server socket, but the server is stuck in a foreverloop.
	if(close(s_socket) == -1)
	{
		perror("close");
		return -1;
	}

	//printf("close sockets.. :)");

	return 0;
}
