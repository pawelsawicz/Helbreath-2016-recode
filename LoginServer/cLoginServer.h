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

#endif