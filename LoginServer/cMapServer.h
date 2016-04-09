#ifndef _cMapServer_H
#define _cMapServer_H

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#include "cSocket.h"
#include "cGlobalDefines.h"

class cMapServer
{
public:
	cMapServer(int index);
	virtual ~cMapServer();

	char serverName[21], mapName[MAXMAPLIST][21];

};
struct cMapSocket {
	USHORT port;
	IN_ADDR addr;
	unsigned int wmEvent = WM_MAP_SOCKET;
	int MaxConnections = MAXMAPSOCKETS;
	ULONG BlockingMode = 1;
	SOCKET listenSocket = NULL;
	SOCKET acceptedSockets[MAXMAPSOCKETS];
};

#endif