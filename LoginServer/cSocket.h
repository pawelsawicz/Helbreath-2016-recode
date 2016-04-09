#ifndef _cSocket_H
#define _cSocket_H


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>

#include "Application.h"
#include "cGlobalDefines.h"

class cSocket
{
public:
	cSocket();
	virtual ~cSocket();

	int Initialize();
	SOCKET ListenSocket(char * ip, u_short port, ULONG BlockingMode, int MaxConnections, unsigned int wmEvent);
	SOCKET AcceptSocket(SOCKET ListenSocket, unsigned int wmEvent);
	SOCKET ConnectSocket(char * ip, u_short port, unsigned int wmEvent);
	int cSocket::SendSocket(SOCKET acceptedSock, char * data, int Size);
	int RecvSocket(SOCKET acceptedSock, char * buf, int len);

	WSADATA wsaData;
};

#endif