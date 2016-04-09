#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include "Game.h"

#pragma comment(lib, "Ws2_32.lib")

class Sockets
{
public:
	Sockets();
	~Sockets();

	WSADATA wsaData;

	int Initialize();
	int CreateSocket();
	int BindSocket(SOCKET ListenSocket, char * ip, u_short port);
	int ListenSocket(SOCKET ListenSocket, unsigned int wmEvent);
	int AcceptSocket(SOCKET ListenSocket, unsigned int wmEvent);
	int ConnectSocket(SOCKET ConnectSocket, char * ip, u_short port, unsigned int wmEvent);
	//accept
	int ReceiveIncomingMessages(SOCKET ClientSocket, unsigned int wmEvent);
	int iSendMsg(SOCKET ClientSocket, char * cData, DWORD dwSize, char cKey);
	int _iSend(SOCKET ClientSocket, char * cData, int iSize);

	int _iOnRead(SOCKET ClientSocket);

	char   m_cStatus;
	DWORD  m_dwReadSize;
	DWORD  m_dwTotalReadSize;

	char * m_pRcvBuffer;
	char * m_pSndBuffer;
	DWORD  m_dwBufferSize;

	SOCKET LoginSocket, ClientSocket;
};

