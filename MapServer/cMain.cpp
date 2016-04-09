#include "cMain.h"

cMain::cMain()
{
	// Creates long-lived sockets by assigning them dynamic memory.
	G_pListenSock = new cSocket;
	G_pListenSock->Initialize(); // Setup winsock
	G_pLoginSock = new struct cLoginSocket();
	G_pClientSock = new struct cClientSocket();
	for (int i = 0; i < MAXCLIENTSOCKETS; i++) {
		G_pClientSock->acceptedSockets[i] = NULL;
	}
}


cMain::~cMain()
{
	delete G_pListenSock;
	delete G_pLoginSock;
	delete G_pClientSock;
}

void cMain::StartUp()
{
	char Log[120];

	DisplayInLogList("(*) Starting Server!");
	bReadConfigFile("MapServer.cfg");

	// Listen for client connections.
	G_pClientSock->listenSocket = G_pListenSock->ListenSocket(IpAddress, G_pClientSock->port, G_pClientSock->BlockingMode, G_pClientSock->MaxConnections, G_pClientSock->wmEvent);
	if (G_pClientSock->listenSocket == NULL){
		wsprintf(Log, "(ERROR) Failed Listening For Client: %ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}
	else {
		wsprintf(Log, "(*) Listening For Client: %ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}

	DisplayInLogList("");
	DisplayInLogList("(*) Server Started! Press HOME to register this map server on the login server.");
}
void cMain::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	char Log[120];
	DWORD * dwp;
	char  SendData[15];
	WORD  * wp;
	short * sp;
	char  * cp;

	switch (wParam) {
		//Home key pressed
	case VK_HOME:
		// Register this map server on the login server.
		if (G_pLoginSock->connectSocket == NULL) {
			G_pLoginSock->connectSocket = G_pListenSock->ConnectSocket(IpAddress, G_pLoginSock->port, G_pLoginSock->wmEvent);
			if (G_pLoginSock->connectSocket == NULL){
				wsprintf(Log, "(ERROR) Failed to connect Login Server On Socket: %ld Port:%d\n", G_pLoginSock->connectSocket, G_pLoginSock->port);
				DisplayInLogList(Log);
			}
			else {
				wsprintf(Log, "(*) Connected To Login Server On Socket: %ld Port:%d\n", G_pLoginSock->connectSocket, G_pLoginSock->port);
				DisplayInLogList(Log);
			}

			break;

	case 0x53://s
		ZeroMemory(SendData, sizeof(SendData));

		dwp = (DWORD*)SendData;
		*dwp = MSGID_TEST_MSG;

		wp = (WORD *)(SendData + 4);
		*wp = MSGTYPE_CONFIRM;

		cp = (char *)(SendData + 8);

		wp = (WORD *)cp;
		*wp = 32;
		cp += 2;

		sp = (short *)cp;
		*sp = 23;
		cp += 2;

		sp = (short *)cp;
		*sp = 35;
		cp += 2;

		G_pListenSock->SendSocket(G_pLoginSock->connectSocket, SendData, 14);
		break;

		}
	}
}

void cMain::SocketsMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	int WSAEvent;
	char Log[120];

	/*	FD_READ		notifies when data arrives at our socket
		FD_WRITE	receives notifications of readiness for writing
		FD_ACCEPT	notifies of events for incoming connections
		FD_CONNECT	notifies of events for socket connections
		FD_CLOSE	alerts when a socket has been closed
		*/
	WSAEvent = WSAGETSELECTEVENT(lParam);
	switch (message)
	{
	case WM_LOGIN_SOCKET:
		switch (WSAEvent) {
		case FD_READ:
			msgDecode();
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			break;
		case FD_CONNECT:
			break;
		case FD_CLOSE:
			break;
		}
		break;
	case WM_CLIENT_SOCKET:
		switch (WSAEvent) {
		case FD_READ:
			msgDecode();
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			// A client requests to connect to the map server. Accept it.
			for (UINT i = 0; i < MAXCLIENTSOCKETS; i++){
				if (G_pClientSock->acceptedSockets[i] == NULL){
					G_pClientSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pClientSock->listenSocket, message);
					wsprintf(Log, "Accepted client (%d/%d). Socket: %ld\n", i + 1, MAXCLIENTSOCKETS, G_pClientSock->acceptedSockets[i]);
					DisplayInLogList(Log);
					return;
				}
				wsprintf(Log, "(ERROR) Cannot accept any more client. Limit reached (%d/%d). \n", MAXCLIENTSOCKETS, MAXCLIENTSOCKETS);
				DisplayInLogList(Log);
			}
			break;
		case FD_CONNECT:
			break;
		case FD_CLOSE:
			break;
		}
		break;
	}
}

bool cMain::bReadConfigFile(char * filename)
{
	FILE * pFile;
	HANDLE hFile;
	DWORD  dwFileSize;
	char * cp, *token, cTxt[120];
	ConfigFileReadMode cReadMode = ConfigFileReadMode::Undefined;
	char seps[] = "= ,\t\n";
	int	iIPindex = 0;

	hFile = CreateFile(filename, GENERIC_READ, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	dwFileSize = GetFileSize(hFile, NULL);
	if (hFile != INVALID_HANDLE_VALUE) CloseHandle(hFile);


	pFile = fopen(filename, "rt");
	if (pFile == NULL) {

		DisplayInLogList("(ERROR) Cannot open configuration file.");
		return FALSE;
	}
	else {
		DisplayInLogList("(*) Reading configuration file...");
		cp = new char[dwFileSize + 1];
		ZeroMemory(cp, dwFileSize + 1);
		fread(cp, dwFileSize, 1, pFile);

		token = strtok(cp, seps);
		while (token != NULL)   {

			if (cReadMode != ConfigFileReadMode::Undefined) {
				switch (cReadMode) {
				case ConfigFileReadMode::ServerName:
					ZeroMemory(ServerName, sizeof(ServerName));
					strcpy(ServerName, token);
					wsprintf(cTxt, "(*) Server Name : %s", ServerName);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::Address:
					ZeroMemory(IpAddress, sizeof(IpAddress));
					strcpy(IpAddress, token);
					wsprintf(cTxt, "(*) Server Ip Address : %s", IpAddress);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::ServerPort:
					G_pClientSock->port = atoi(token);
					wsprintf(cTxt, "(*) Server Port : %d", G_pClientSock->port);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::LoginPort:
					G_pLoginSock->port = atoi(token);
					wsprintf(cTxt, "(*) Login Server Port : %d", G_pLoginSock->port);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::Version:
					ZeroMemory(Version, sizeof(Version));
					strcpy(Version, token);
					wsprintf(cTxt, "(*) Server Version : %s", Version);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				}

			}
			else {
				if (memcmp(token, "server-name", 11) == 0) cReadMode = ConfigFileReadMode::ServerName;
				if (memcmp(token, "address", 7) == 0)     cReadMode = ConfigFileReadMode::Address;
				if (memcmp(token, "server-port", 11) == 0) cReadMode = ConfigFileReadMode::ServerPort;
				if (memcmp(token, "login-port", 10) == 0) cReadMode = ConfigFileReadMode::LoginPort;
				if (memcmp(token, "version", 7) == 0) cReadMode = ConfigFileReadMode::Version;
			}
			token = strtok(NULL, seps);
		}

		delete cp;
	}
	if (pFile != NULL) fclose(pFile);
	DisplayInLogList("(*) Reading configuration file successful");
	DisplayInLogList("");
	return TRUE;

}
void cMain::msgDecode()
{
	const int recvbuflen = 512;
	char recvbuf[512] = "";
	int byteReceived;
	DWORD  *dwpMsgID;;
	WORD * wpResult;
	short * sp;
	char  * cp;
	short Temp1, Temp2, Temp3;
	char Log[120];

	memset(recvbuf, 0, sizeof recvbuf);
	byteReceived = G_pListenSock->RecvSocket(G_pLoginSock->connectSocket, recvbuf, recvbuflen);
	wsprintf(Log, "Byte received:%d data:%s\n", byteReceived, recvbuf);
	DisplayInLogList(Log);

	dwpMsgID = (DWORD *)(recvbuf);
	switch (*dwpMsgID) {

	case MSGID_TEST_MSG:
		DisplayInLogList("MSGID_TEST_MSG");
		wpResult = (WORD *)(recvbuf + 4);
		switch (*wpResult) {
		case MSGTYPE_CONFIRM:
			DisplayInLogList("MSGTYPE_CONFIRM");
			break;
		case MSGTYPE_REJECT:
			DisplayInLogList("MSGTYPE_REJECT");
			break;
		}
		cp = (char *)(recvbuf + 8);

		sp = (short *)cp;
		Temp1 = *sp;
		cp += 2;

		sp = (short *)cp;
		Temp2 = *sp;
		cp += 2;

		sp = (short *)cp;
		Temp3 = *sp;
		cp += 2;
		wsprintf(Log, "Temp1: %d Temp2: %d Temp3: %d \n", Temp1, Temp2, Temp3);
		DisplayInLogList(Log);
		break;
	}
}