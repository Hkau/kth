#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "levelfromserver.h"
#define PORT 5555
#define DEFAULT_PORT "5555"

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

#ifdef _WIN32
LevelFromServer::LevelFromServer(const char*address) :
	status(0),
	ConnectSocket(INVALID_SOCKET)

{
	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		status = 1;
		return;
	}

	// Create socket
	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(address, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		status = 2;
		return;
	}

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr=result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
						   ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		status = 3;
		return;
	}

	// Connect to server.
	iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		status = 4;
		return;
	}
}

#else //linux/bsd/anything non-shit
LevelFromServer::LevelFromServer(const char*address) :
	status(0)
{
	int n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[BUFFERSIZE];

	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ConnectSocket < 0)
		error("ERROR opening socket");
	server = gethostbyname(address);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		 (char *)&serv_addr.sin_addr.s_addr,
		 server->h_length);
	serv_addr.sin_port = htons(PORT);
	if (connect(ConnectSocket,(const struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");
}

#endif

const char * LevelFromServer::getLevel(int boardnr)
{
	char boardstr[10];
	sprintf(boardstr,"%d\n",boardnr);
	//n = write(sockfd,boardstr,strlen(boardstr));
	int n = send(ConnectSocket,boardstr,strlen(boardstr),0);
	if (n < 0)
		error("ERROR writing to socket");

	memset(buffer,0,BUFFERSIZE);
	//n = read(sockfd,buffer,BUFFERSIZE-1);
	n = recv(ConnectSocket,buffer,BUFFERSIZE-1, 0);
	if (n < 0)
		error("ERROR reading from socket");

	return buffer;
}

const char * LevelFromServer::sendSolution(const char *solution)
{
	//char *MYSOL="0 3 3 1 0 0 2 1 2 2 0 2 2 1 3 3 3 3 2 1 1 3 0 3 0 1 2 2 0 3";
	//n = write(sockfd,MYSOL,strlen(MYSOL));
	int n = send(ConnectSocket,solution,strlen(solution), 0);
	n = send(ConnectSocket,"\n",1, 0);
	if (n < 0)
		error("ERROR writing to socket");
	memset(buffer,0,BUFFERSIZE);
	//n = read(sockfd,buffer,BUFFERSIZE-1);
	n = recv(ConnectSocket,buffer,BUFFERSIZE-1, 0);
	//printf("%s\n",buffer);
	return buffer;
}

#ifdef _WIN32
LevelFromServer::~LevelFromServer()
{
	// cleanup
	if (ConnectSocket != INVALID_SOCKET)
		closesocket(ConnectSocket);
	WSACleanup();
}

#else // fu :3
LevelFromServer::~LevelFromServer()
{
	// cleanup
//	if (ConnectSocket != INVALID_SOCKET)
//		closesocket(ConnectSocket);
//	WSACleanup();
}
#endif
