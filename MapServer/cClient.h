#ifndef _cClient_H
#define _cClient_H
#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#include "cSocket.h"
#include "cGlobalDefines.h"

class cClient
{
public:
	cClient(int index);
	virtual ~cClient();

	char accountName[21], accountPassword[21], charName[21];

	

};
struct cClientSocket {
	USHORT port;
	IN_ADDR addr;
	unsigned int wmEvent = WM_CLIENT_SOCKET;
	int MaxConnections = MAXCLIENTSOCKETS;
	ULONG BlockingMode = 1;
	SOCKET listenSocket;
	SOCKET acceptedSockets[MAXCLIENTSOCKETS];
};
#endif