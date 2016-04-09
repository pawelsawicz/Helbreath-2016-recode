#ifndef _cMain_H
#define _cMain_H

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#include "Application.h"
#include "cClient.h"
#include "cLoginServer.h"
#include "cSocket.h"
#include "../shared/NetworkMessages.h"

class cMain
{
public:
	cMain();
	//virtual ~cMain();
	~cMain();

	void StartUp();
	bool bReadConfigFile(char * filename);
	void OnKeyUp(WPARAM wParam, LPARAM lParam);
	void SocketsMessages(UINT message, WPARAM wParam, LPARAM lParam);
	void msgDecode();

	char IpAddress[46], Version[6], ServerName[36];

	class cSocket * G_pListenSock = NULL;
	struct cLoginSocket * G_pLoginSock = NULL;
	struct cClientSocket * G_pClientSock = NULL;

	// Config file read modes.
	enum class ConfigFileReadMode { Undefined, ServerName, Address, ServerPort, LoginPort, Version };

};

#endif