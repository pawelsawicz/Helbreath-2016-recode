//#include "stdafx.h"
#include "../Header/Sockets.h"

extern HWND hWnd;
//Info//

//Server

//Initialize Winsock.
//Create a socket.
//Bind the socket.
//Listen on the socket for a client.
//Accept a connection from a client.
//Receive and send data.
//Disconnect.

//Client
//
//Initialize Winsock.
//Create a socket.
//Connect to the server.
//Send and receive data.
//Disconnect.

//Info//

Sockets::Sockets()
{

	LoginSocket = NULL;

}


Sockets::~Sockets()
{
}

int Sockets::Initialize()
{
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	return 0;
}

int Sockets::CreateSocket()
{
	//Application Application;
	SOCKET ListenSocket = INVALID_SOCKET;
	char Log[120];

	// Create a SOCKET for the server to listen for client connections
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET) {
		wsprintf(Log, "Error at socket(): %ld\n", WSAGetLastError());
		//Application.DisplayInLogList(Log);
		WSACleanup();
		return 1;
	}
	return ListenSocket;
}
int Sockets::BindSocket(SOCKET ListenSocket, char * ip, u_short port)
{
	//Application Application;
	struct sockaddr_in saServer;
	int iResult;
	char Log[120];

	// Set up the sockaddr structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(ip);
	saServer.sin_port = htons(port);

	// Setup the TCP listening socket
	// Bind the listening socket using the
	// information in the sockaddr structure
	iResult = bind(ListenSocket, (SOCKADDR*)&saServer, sizeof(saServer));

	if (iResult == SOCKET_ERROR) {
		wsprintf(Log, "bind failed with error: %d\n", WSAGetLastError());
		//Application.DisplayInLogList(Log);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	return 0;
}
int Sockets::ListenSocket(SOCKET ListenSocket, unsigned int wmEvent)
{
	//Application Application;
	char Log[120];

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		wsprintf(Log, "Listen failed with error: %ld\n", WSAGetLastError());
		//Application.DisplayInLogList(Log);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	WSAAsyncSelect(ListenSocket, hWnd, wmEvent, FD_ACCEPT);

	return 0;
}
int Sockets::AcceptSocket(SOCKET ListenSocket, unsigned int wmEvent)
{
	//Application Application;
	SOCKET ClientSocket = INVALID_SOCKET;
	char Log[120];

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		wsprintf(Log, "accept failed: %d\n", WSAGetLastError());
		//Application.DisplayInLogList(Log);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	WSAAsyncSelect(ClientSocket, hWnd, wmEvent, FD_READ | FD_WRITE | FD_CLOSE);

	return ClientSocket;
}
int Sockets::ConnectSocket(SOCKET ConnectSocket, char * ip, u_short port, unsigned int wmEvent)
{
	//Application Application;
	struct sockaddr_in saServer;
	int iResult;
	char Log[120];

	// Set up the sockaddr structure
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = inet_addr(ip);
	saServer.sin_port = htons(port);

	// Setup the TCP listening socket
	// Bind the listening socket using the
	// information in the sockaddr structure
	iResult = connect(ConnectSocket, (SOCKADDR*)&saServer, sizeof(saServer));

	if (iResult == SOCKET_ERROR) {
		wsprintf(Log, "Connect failed with error: %d\n", WSAGetLastError());
		//Application.DisplayInLogList(Log);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	WSAAsyncSelect(ClientSocket, hWnd, wmEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	return 0;
}
int Sockets::ReceiveIncomingMessages(SOCKET ClientSocket, unsigned int wmEvent)
{
	return 0;
}
int Sockets::_iOnRead(SOCKET ClientSocket)
{
	int iRet;
	WORD  * wp;
	char Log[120];
//	Application Application;

	if (m_cStatus == XSOCKSTATUS_READINGHEADER) {

		iRet = recv(ClientSocket, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);

		if (iRet == SOCKET_ERROR) {
			//wsprintf(Log, "bind failed with error: %d\n", WSAGetLastError());
			//Application.DisplayInLogList(Log);
			closesocket(ClientSocket);
			WSACleanup();
			return false;
		}

		m_dwReadSize -= iRet;
		m_dwTotalReadSize += iRet;

		if (m_dwReadSize == 0) {
			m_cStatus = XSOCKSTATUS_READINGBODY;
			wp = (WORD *)(m_pRcvBuffer + 1);
			m_dwReadSize = (int)(*wp - 3);

			if (m_dwReadSize == 0) {
				m_cStatus = XSOCKSTATUS_READINGHEADER;
				m_dwReadSize = 3;
				m_dwTotalReadSize = 0;
				return true;
			}
		}
		return true;
	}
	else
	if (m_cStatus == XSOCKSTATUS_READINGBODY) {

		iRet = recv(ClientSocket, (char *)(m_pRcvBuffer + m_dwTotalReadSize), m_dwReadSize, 0);

		if (iRet == SOCKET_ERROR) {
			//wsprintf(Log, "bind failed with error: %d\n", WSAGetLastError());
			//Application.DisplayInLogList(Log);
			closesocket(ClientSocket);
			WSACleanup();
			return false;
		}

		m_dwReadSize -= iRet;
		m_dwTotalReadSize += iRet;

		if (m_dwReadSize == 0) {
			m_cStatus = XSOCKSTATUS_READINGHEADER;
			m_dwReadSize = 3;
			m_dwTotalReadSize = 0;
		}
		else return true;
	}

	return true;
}
int Sockets::iSendMsg(SOCKET ClientSocket, char * cData, DWORD dwSize, char cKey)
{
	WORD * wp;
	int    iRet;
	int  i;

	if (dwSize > m_dwBufferSize) return false;

	m_pSndBuffer[0] = cKey;

	wp = (WORD *)(m_pSndBuffer + 1);
	*wp = (WORD)(dwSize + 3);

	memcpy((char *)(m_pSndBuffer + 3), cData, dwSize);

	for (i = 0; i < dwSize; i++) {
		m_pSndBuffer[3 + i] += (i ^ cKey);
		m_pSndBuffer[3 + i] = (char)(m_pSndBuffer[3 + i] ^ (cKey ^ (dwSize - i)));
	}

	iRet = _iSend(ClientSocket, m_pSndBuffer, dwSize + 3);

	if (iRet < 0) return iRet;
	else return (iRet - 3);
}
int Sockets::_iSend(SOCKET ClientSocket, char * cData, int iSize)
{
	int  iOutLen, iRet;
	char Log[120];
	//Application Application;

	iOutLen = 0;
	while (iOutLen < iSize) {

		iRet = send(ClientSocket, (cData + iOutLen), iSize - iOutLen, 0);

		if (iRet == SOCKET_ERROR) {
			//wsprintf(Log, "bind failed with error: %d\n", WSAGetLastError());
			//Application.DisplayInLogList(Log);
			closesocket(ClientSocket);
			WSACleanup();
			return false;
		}
		iOutLen += iRet;
	}
	return iOutLen;
}
