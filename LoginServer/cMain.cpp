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
	UINT iTmp;
	WORD ID;


	/*  FD_READ		notifies when data arrives at our socket
		FD_WRITE	receives notifications of readiness for writing
		FD_ACCEPT	notifies of events for incoming connections
		FD_CONNECT	notifies of events for socket connections
		FD_CLOSE	alerts when a socket has been closed
		*/
	WSAEvent = WSAGETSELECTEVENT(lParam);

	if ((message >= WM_MAP_SOCKET) && (message < WM_MAP_SOCKET + MAXMAPSOCKETS)){
		iTmp = WM_MAP_SOCKET;
		ID = (WORD)(message - iTmp);

		switch (WSAEvent) {
		case FD_READ:
			IncomingMapServerMessages(ID);
			break;
		case FD_WRITE:
			break;

		case FD_ACCEPT:
			// A map server requests to connect to the login server. Accept it.
			for (UINT i = 0; i < MAXMAPSOCKETS; i++){
				if (G_pMapSock->acceptedSockets[i] == NULL){
					G_pMapSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pMapSock->listenSocket, message + i);
					wsprintf(Log, "Accepted map server (%d/%d). Socket: %ld Message: %d\n", i + 1, MAXMAPSOCKETS, G_pMapSock->acceptedSockets[i], message + i);
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
	}
	else if ((message >= WM_CLIENT_SOCKET) && (message < WM_CLIENT_SOCKET + MAXCLIENTSOCKETS)){
		iTmp = WM_CLIENT_SOCKET;
		ID = (WORD)(message - iTmp);

		switch (WSAEvent) {
		case FD_READ:
			IncomingClientMessages(ID);
			break;
		case FD_WRITE:
			break;
		case FD_ACCEPT:
			// A client requests to connect to the login server. Accept it.
			for (UINT i = 0; i < MAXCLIENTSOCKETS; i++){
				if (G_pClientSock->acceptedSockets[i] == NULL){
					G_pClientSock->acceptedSockets[i] = G_pListenSock->AcceptSocket(G_pClientSock->listenSocket, message + i);
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
void cMain::IncomingClientMessages(WORD SocketID)
{
	const int recvbuflen = 512;
	char recvbuf[512] = "";
	int byteReceived;
	DWORD  *dwpMsgID;
	char Log[120];
	MYSQL myConn;

	memset(recvbuf, 0, sizeof recvbuf);
	byteReceived = G_pListenSock->RecvSocket(G_pClientSock->acceptedSockets[SocketID], recvbuf, recvbuflen);
	wsprintf(Log, "Byte received:%d data:%s\n", byteReceived, recvbuf);
	DisplayInLogList(Log);

	mysql_init(&myConn);
	if (!mysql_real_connect(&myConn, IpAddress, MysqlUserName, MysqlPassword, MysqlDatabaseName, MysqlPort, NULL, NULL)){
		MyAux_Get_Error(&myConn);
		mysql_close(&myConn);
		delete[] recvbuf;
		return;
	}

	dwpMsgID = (DWORD *)(recvbuf);
	switch (*dwpMsgID) {

	case MSGID_REQUEST_CREATENEWACCOUNT:
		if (byteReceived < 20){
			delete[] recvbuf;
			mysql_close(&myConn);
			return;
		}
		CreateNewAccount((recvbuf + 6), SocketID, myConn);
		break;

	}
}
void cMain::IncomingMapServerMessages(WORD SocketID)
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
	byteReceived = G_pListenSock->RecvSocket(G_pMapSock->acceptedSockets[SocketID], recvbuf, recvbuflen);
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
int cMain::ProcessQuery(MYSQL *myConn, char *cQuery)
{
	BYTE bErrorCount = 0;
	int iQuery = -1;
	UINT uiLastError;

	do {
		if (mysql_ping(myConn) != 0){
			mysql_close(myConn);
			mysql_init(myConn);
			mysql_real_connect(myConn, IpAddress, MysqlUserName, MysqlPassword, MysqlDatabaseName, MysqlPort, NULL, NULL);
		}
		else iQuery = mysql_query(myConn, cQuery);
		bErrorCount++;
		uiLastError = MyAux_Get_Error(myConn);
		if (uiLastError) Delay(1000);
	} while (uiLastError != NULL && bErrorCount < MAXALLOWEDQUERYERROR);
	if (bErrorCount == MAXALLOWEDQUERYERROR){
		DisplayInLogList(cQuery);
		return -1;
	}
	return iQuery;
}
void cMain::CreateNewAccount(char *Data, WORD ClientID, MYSQL myConn)
{
	char NewAccName[11], NewAcc1[11], CreateDate[25], NewEmailAddr[51], safeEmailAddr[51], safeAccQuiz[46], safeAnswer[21];
	char NewAccountQuiz[46], NewAccountAnswer[21], GoodPass[25], NewPass1[11], Txt50[50], QueryConsult[500];
	DWORD *dwp;
	st_mysql_res    *QueryResult = NULL;
	WORD *wp;
	char Txt100[100], Txt500[500];
	_ADDRESS ClientIP;
	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);


	ZeroMemory(Txt100, sizeof(Txt100));
	ZeroMemory(NewAcc1, sizeof(NewAcc1));
	ZeroMemory(NewPass1, sizeof(NewPass1));

	ZeroMemory(NewEmailAddr, sizeof(NewEmailAddr));
	ZeroMemory(NewAccountQuiz, sizeof(NewAccountQuiz));
	ZeroMemory(NewAccountAnswer, sizeof(NewAccountAnswer));
	sprintf(Txt100, "(!) Create acc in progress.");
	DisplayInLogList(Txt100);

	SafeCopy(NewAcc1, Data, 10);
	SafeCopy(NewPass1, Data + 10, 10);
	SafeCopy(NewEmailAddr, Data + 20, 50);
	SafeCopy(NewAccountQuiz, Data + 70, 45);
	SafeCopy(NewAccountAnswer, Data + 115, 20);

	ZeroMemory(ClientIP, sizeof(ClientIP));

	G_pListenSock->iGetPeerAddress(G_pClientSock->acceptedSockets[ClientID], ClientIP);
	ZeroMemory(CreateDate, sizeof(CreateDate));
	sprintf(CreateDate, "%d-%d-%d %d:%d:%d", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);


	ZeroMemory(Txt500, sizeof(Txt500));
	dwp = (DWORD*)Txt500;
	*dwp = MSGID_RESPONSE_LOG;

	if (AccountExists(NewAcc1, myConn)){
		wp = (WORD*)(Txt500 + 4);
		*wp = LOGRESMSGTYPE_ALREADYEXISTINGACCOUNT;
		//SendMsgToClient(ClientID, Txt500, 6);
	}
	else {
		ZeroMemory(QueryConsult, sizeof(QueryConsult));
		ZeroMemory(NewAccName, sizeof(NewAccName));
		MakeGoodName(NewAccName, NewAcc1);
		ZeroMemory(GoodPass, sizeof(GoodPass));
		MakeGoodName(GoodPass, NewPass1);
		ZeroMemory(safeEmailAddr, sizeof(safeEmailAddr));
		MakeGoodName(safeEmailAddr, NewEmailAddr);
		ZeroMemory(safeAccQuiz, sizeof(safeAccQuiz));
		MakeGoodName(safeAccQuiz, NewAccountQuiz);
		ZeroMemory(safeAnswer, sizeof(safeAnswer));
		MakeGoodName(safeAnswer, NewAccountAnswer);

		sprintf(QueryConsult, "INSERT INTO `account_database` SET `name` = '%s' , `password` = '%s', `LoginIpAddress` = '%s', `CreateDate` = '%s', `Email` = '%s', `Quiz` = '%s', `Answer` = '%s';", NewAccName, GoodPass, ClientIP, CreateDate, safeEmailAddr, safeAccQuiz, safeAnswer);

		if (ProcessQuery(&myConn, QueryConsult) == -1) return;
		QueryResult = mysql_store_result(&myConn);
		mysql_free_result(QueryResult);
		SafeCopy(Txt50, NewAcc1, strlen(NewAcc1));
		SafeCopy(Txt50 + 6, NewPass1, strlen(NewPass1));
		wp = (WORD*)(Txt500 + 4);
		*wp = LOGRESMSGTYPE_NEWACCOUNTCREATED;
		//SendMsgToClient(ClientID, Txt500, 6);
	}
	mysql_free_result(QueryResult);
}
bool cMain::AccountExists(char *AccountName, MYSQL myConn)
{
	char QueryConsult[150], GoodAccountName[50];
	st_mysql_res *QueryResult = NULL;
	MYSQL_ROW Row;

	ZeroMemory(QueryConsult, sizeof(QueryConsult));
	ZeroMemory(GoodAccountName, sizeof(GoodAccountName));
	MakeGoodName(GoodAccountName, AccountName);
	sprintf(QueryConsult, "SELECT `name` FROM `account_database` WHERE `name` = '%s' LIMIT 1;", GoodAccountName);
	if (ProcessQuery(&myConn, QueryConsult) == -1) return FALSE;
	QueryResult = mysql_store_result(&myConn);

	if (mysql_num_rows(QueryResult) == 1){
		Row = mysql_fetch_row(QueryResult);
		*AccountName = atoi(Row[0]);
		mysql_free_result(QueryResult);
		return TRUE;
	}
	mysql_free_result(QueryResult);
	return FALSE;
}
void cMain::SafeCopy(char *c1, char *c2, DWORD lenght)
{
	register DWORD d;

	if (lenght == 0){
		d = 0;
		if (c2[d] == NULL) return;
		while (c2[d] != NULL){
			c1[d] = c2[d];
			d++;
		}
	}
	else for (d = 0; d < lenght; d++) c1[d] = c2[d];

	c1[d] = NULL;
}

bool cMain::IsSame(char *c1, char *c2)
{
	DWORD size1, size2;

	size1 = strlen(c1);
	size2 = strlen(c2);
	if (size1 == size2 && memcmp(c1, c2, size1) == 0) return true;
	else return false;
}

void cMain::MakeGoodName(char *dest, char *source)
{
	DWORD size, d;
	char *cp;

	size = strlen(source);
	if (size == 0) return;
	d = 0;
	cp = (char *)dest;
	while (source[d] != NULL){
		if (source[d] == '\'' || source[d] == '\\' || source[d] == '\"'){

			cp[0] = '\\';
			cp++;
			cp[0] = source[d];
			cp++;
		}
		else{
			cp[0] = source[d];
			cp++;
		}
		d++;
	}
}
void cMain::Delay(DWORD nTimeMs)
{
	MSG msg;
	DWORD endTick;

	endTick = GetTickCount() + nTimeMs;
	while (GetTickCount() < endTick){
		if (PeekMessage(&msg, NULL, 0, 0, TRUE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return;
}