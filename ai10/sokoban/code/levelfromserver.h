#ifndef LEVELFROMSERVER_H
#define LEVELFROMSERVER_H

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#else
#define SOCKET int
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#define BUFFERSIZE 4096

class LevelFromServer
{
private:
	// Don't allow copy-constructor
	LevelFromServer(const LevelFromServer & lvl);

	int status;
	SOCKET ConnectSocket;
	char buffer[BUFFERSIZE];

public:
	LevelFromServer(const char *address);
	~LevelFromServer();
	const char * getLevel(int boardnr);
	const char * sendSolution(const char * solution);
};

#endif // LEVELFROMSERVER_H
