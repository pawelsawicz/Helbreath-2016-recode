#ifndef _cLoginServer_H
#define _cLoginServer_H

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#include "cSocket.h"
#include "cGlobalDefines.h"

class cLoginServer
{
public:
	cLoginServer();
	virtual ~cLoginServer();

	int index;
	USHORT port;
	IN_ADDR addr;

};

struct cLoginSocket {
	USHORT port;
	IN_ADDR addr;
	unsigned int wmEvent = WM_LOGIN_SOCKET;
	SOCKET connectSocket = NULL;
};

#endif