#ifndef _cMain_H
#define _cMain_H

#define _WINSOCKAPI_   /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>
#include "Application.h"
#include "cClient.h"
#include "cLoginServer.h"
#include "cMapServer.h"
#include "cSocket.h"
#include "../shared/NetworkMessages.h"
#include "mysql/mysql.h"

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
	void IncomingClientMessages(WORD SocketID);
	void IncomingMapServerMessages(WORD SocketID);
	UINT MyAux_Get_Error(struct st_mysql *pmySql);
	bool AccountExists(char *AccountName, MYSQL myConn);
	void CreateNewAccount(char *Data, WORD ClientID, MYSQL myConn);
	void MakeGoodName(char *dest, char *source);
	bool IsSame(char *c1, char *c2);
	void SafeCopy(char *c1, char *c2, DWORD lenght);
	int ProcessQuery(MYSQL *myConn, char *cQuery);
	void Delay(DWORD nTimeMs);

	char IpAddress[46], Version[6], ServerList[36];
	char MysqlDatabaseName[46], MysqlUserName[21], MysqlPassword[21];
	int MysqlPort;

	class cSocket * G_pListenSock;
	struct cMapSocket * G_pMapSock;
	struct cClientSocket * G_pClientSock;
	

	// Config file read modes.
	enum class ConfigFileReadMode { Undefined, Address, ClientPort, MapPort, Version, ServerList, MysqlDatabaseName, MysqlUserName, MysqlPassword, MysqlPort };
};

#endif