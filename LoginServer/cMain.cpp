#include "cMain.h"

class cSocket * G_pListenSock = NULL;
struct cMapSocket * G_pMapSock = NULL;
struct cClientSocket * G_pClientSock = NULL;
MYSQL mySQL;

cMain::cMain()
{
	// Creates long-lived sockets by assigning them dynamic memory.
	G_pListenSock = new cSocket;
	G_pListenSock->Initialize(); // Setup winsock
	G_pMapSock = new struct cMapSocket();
	for (int i = 0; i < MAXMAPSOCKETS; i++) {
		G_pMapSock->acceptedSockets[i] = NULL;
	}
	G_pClientSock = new struct cClientSocket();
	for (int i = 0; i < MAXCLIENTSOCKETS; i++) {
		G_pClientSock->acceptedSockets[i] = NULL;
	}
}


cMain::~cMain()
{
	delete G_pListenSock;
	delete G_pMapSock;
	delete G_pClientSock;
}

void cMain::StartUp()
{
	char Log[120];
	UINT iResult = 0;

	DisplayInLogList("(*) Starting Server!");
	bReadConfigFile("LoginServer.cfg");

	// Set map socket to listen for incoming connections to the map port. Note that it has not started accepting any connection yet.
	G_pMapSock->listenSocket = G_pListenSock->ListenSocket(IpAddress, G_pMapSock->port, G_pMapSock->BlockingMode, G_pMapSock->MaxConnections, G_pMapSock->wmEvent);
	if (G_pMapSock->listenSocket == NULL){
		wsprintf(Log, "(ERROR) Listening For Map Server. Socket:%ld Port:%d\n", G_pMapSock->listenSocket, G_pMapSock->port);
		DisplayInLogList(Log);
	}
	else {
		wsprintf(Log, "(*) Listening For Map Server. Socket:%ld Port:%d\n", G_pMapSock->listenSocket, G_pMapSock->port);
		DisplayInLogList(Log);
	}

	// Set client socket to listen for incoming connections to the client port. Note that it has not started accepting any connection yet.
	G_pClientSock->listenSocket = G_pListenSock->ListenSocket(IpAddress, G_pClientSock->port, G_pClientSock->BlockingMode, G_pClientSock->MaxConnections, G_pClientSock->wmEvent);
	if (G_pClientSock->listenSocket == NULL){
		wsprintf(Log, "(ERROR) Listening For Client. Socket:%ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}
	else {
		wsprintf(Log, "(*) Listening For Client. Socket:%ld Port:%d\n", G_pClientSock->listenSocket, G_pClientSock->port);
		DisplayInLogList(Log);
	}

	mysql_init(&mySQL);
	if (!mysql_real_connect(&mySQL, IpAddress, MysqlUserName, MysqlPassword, MysqlDatabaseName, MysqlPort, NULL, NULL)){
		iResult = MyAux_Get_Error(&mySQL);
		mysql_close(&mySQL);
	}
	if (iResult != NULL){
		if (iResult == 2003) 
			DisplayInLogList("(!!!) mySql server seems to be offline, please check the IP");
		mysql_close(&mySQL);
		return;
	}
	DisplayInLogList("-Connection to mySQL database was sucessfully established!");

	DisplayInLogList("");
	DisplayInLogList("(*) Server Started!");
}
void cMain::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	char Log[120];

	char  * cp;
	DWORD * dwp;
	WORD  * wp;
	short * sp;
	int iRet;
	char  cData[500];
	short Info1, Info2, Info3;
	Info1 = 324;
	Info2 = 800;
	Info3 = 2;

	switch (wParam) {
		//Home key pressed
	case VK_HOME:
		wsprintf(Log, "Map Server listen socket: %ld\n", G_pMapSock->listenSocket);
		DisplayInLogList(Log);
		break;


	case 0x53://s
		dwp = (DWORD *)(cData);//+4
		*dwp = MSGID_TEST_MSG;

		wp = (WORD *)(cData + 4);//+2
		*wp = MSGTYPE_CONFIRM;

		cp = (char *)(cData + 6);//Starts At +6 Send Data

		sp = (short *)cp;
		*sp = Info1;
		cp += 4;

		sp = (short *)cp;
		*sp = Info2;
		cp += 4;

		*cp = Info3;
		cp++;

		iRet = G_pListenSock->SendSocket(G_pMapSock->acceptedSockets[1], cData, 6 + 9);

		wsprintf(Log, "Send Message on socket: %ld\n", G_pMapSock->acceptedSockets[1]);
		DisplayInLogList(Log);
		break;
	}
}

void cMain::SocketsMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	int WSAEvent;
	char Log[120];


	/*  FD_READ		notifies when data arrives at our socket
		FD_WRITE	receives notifications of readiness for writing
		FD_ACCEPT	notifies of events for incoming connections
		FD_CONNECT	notifies of events for socket connections
		FD_CLOSE	alerts when a socket has been closed
		*/
	WSAEvent = WSAGETSELECTEVENT(lParam);
	switch (message)
	{
	case WM_MAP_SOCKET:
		switch (WSAEvent) {
		case FD_READ:
			msgDecode();
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			// A map server requests to connect to the login server. Accept it.
			for (UINT i = 0; i < MAXMAPSOCKETS; i++){
				if (G_pMapSock->acceptedSockets[i] == NULL){
					G_pMapSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pMapSock->listenSocket, message);
					wsprintf(Log, "Accepted map server (%d/%d). Socket: %ld\n", i + 1, MAXMAPSOCKETS, G_pMapSock->acceptedSockets[i]);
					DisplayInLogList(Log);
					return;
				}
			}
			wsprintf(Log, "(ERROR) Cannot accept any more map server. Limit reached (%d/%d). \n", MAXMAPSOCKETS, MAXMAPSOCKETS);
			DisplayInLogList(Log);
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
			// A client requests to connect to the login server. Accept it.
			for (UINT i = 0; i < MAXCLIENTSOCKETS; i++){
				if (G_pClientSock->acceptedSockets[i] == NULL){
					G_pClientSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pClientSock->listenSocket, message);
					wsprintf(Log, "Accepted client (%d/%d). Socket: %ld\n", i + 1, MAXCLIENTSOCKETS, G_pClientSock->acceptedSockets[i]);
					DisplayInLogList(Log);
					return;
				}
			}
			wsprintf(Log, "(ERROR) Cannot accept any more client. Limit reached (%d/%d). \n", MAXCLIENTSOCKETS, MAXCLIENTSOCKETS);
			DisplayInLogList(Log);
			break;
		case FD_CONNECT:
			wsprintf(Log, "SocketsMessages: WM_MAP_SOCKET FD_CONNECT");
			DisplayInLogList(Log);
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
				case ConfigFileReadMode::Address:
					ZeroMemory(IpAddress, sizeof(IpAddress));
					strcpy(IpAddress, token);
					wsprintf(cTxt, "(*) Server Ip Address : %s", IpAddress);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::ClientPort:
					G_pClientSock->port = atoi(token);
					wsprintf(cTxt, "(*) Client Port : %d", G_pClientSock->port);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::MapPort:
					G_pMapSock->port = atoi(token);
					wsprintf(cTxt, "(*) Map Server Port : %d", G_pMapSock->port);
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
				case ConfigFileReadMode::ServerList:
					ZeroMemory(ServerList, sizeof(ServerList));
					strcpy(ServerList, token);
					wsprintf(cTxt, "(*) Server List Name : %s", ServerList);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::MysqlDatabaseName:
					ZeroMemory(MysqlDatabaseName, sizeof(MysqlDatabaseName));
					strcpy(MysqlDatabaseName, token);
					wsprintf(cTxt, "(*)  Mysql Database Name : %s", MysqlDatabaseName);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::MysqlUserName:
					ZeroMemory(MysqlUserName, sizeof(MysqlUserName));
					strcpy(MysqlUserName, token);
					wsprintf(cTxt, "(*)  Mysql User Name : %s", MysqlUserName);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::MysqlPassword:
					ZeroMemory(MysqlPassword, sizeof(MysqlPassword));
					strcpy(MysqlPassword, token);
					wsprintf(cTxt, "(*)  Mysql Password : %s", MysqlPassword);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				case ConfigFileReadMode::MysqlPort:
					MysqlPort = atoi(token);
					wsprintf(cTxt, "(*) Mysql port : %d", MysqlPort);
					DisplayInLogList(cTxt);
					cReadMode = ConfigFileReadMode::Undefined;
					break;
				}

			}
			else {
				if (memcmp(token, "address", 7) == 0)     cReadMode = ConfigFileReadMode::Address;
				if (memcmp(token, "client-port", 11) == 0) cReadMode = ConfigFileReadMode::ClientPort;
				if (memcmp(token, "map-port", 8) == 0) cReadMode = ConfigFileReadMode::MapPort;
				if (memcmp(token, "version", 7) == 0) cReadMode = ConfigFileReadMode::Version;
				if (memcmp(token, "server-list", 11) == 0) cReadMode = ConfigFileReadMode::ServerList;
				if (memcmp(token, "mysql-database-name", 19) == 0) cReadMode = ConfigFileReadMode::MysqlDatabaseName;
				if (memcmp(token, "mysql-user-name", 15) == 0) cReadMode = ConfigFileReadMode::MysqlUserName;
				if (memcmp(token, "mysql-password", 14) == 0) cReadMode = ConfigFileReadMode::MysqlPassword;
				if (memcmp(token, "mysql-port", 10) == 0) cReadMode = ConfigFileReadMode::MysqlPort;
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
	byteReceived = G_pListenSock->RecvSocket(G_pMapSock->acceptedSockets[0], recvbuf, recvbuflen);
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
UINT cMain::MyAux_Get_Error(struct st_mysql *pmySql)
{
	char ErrMsg[300];
	UINT ErrNum;

	ErrNum = mysql_errno(pmySql);
	if (ErrNum != NULL)
	{
		ZeroMemory(ErrMsg, sizeof(ErrMsg));
		sprintf(ErrMsg, "(!!!) MySql ERROR: [%lu] - %s", ErrNum, mysql_error(pmySql));
		DisplayInLogList(ErrMsg);
		return ErrNum;
	}
	else return NULL;
}