#include "cSocket.h"

extern HWND hWnd;

cSocket::cSocket()
{
}


cSocket::~cSocket()
{
}
int cSocket::Initialize()
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

/**
* Binds the server socket and start listening for clients.
* This should be called by a server.
*
* @param ip server ip
* @param port server port
* @param BlockingMode
* @param MaxConnections
* @param wmEvent event message for hWnd to handle the events on this socket
* @return the server socket.
*/
SOCKET cSocket::ListenSocket(char * ip, u_short port, ULONG BlockingMode, int MaxConnections, unsigned int wmEvent)
{
	struct sockaddr_in saServer;
	int iResult;
	char Log[120];
	SOCKET ListenSocket;

	//Creates Socket For Listen For Client Connections
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET) {
		wsprintf(Log, "Error at socket(): %ld\n", WSAGetLastError());
		DisplayInLogList(Log);
		return false;
	}

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
		DisplayInLogList(Log);
		closesocket(ListenSocket);
		return false;
	}

	// Prepare a socket to listen for connections
	if (listen(ListenSocket, MaxConnections) == SOCKET_ERROR) {
		wsprintf(Log, "Listen failed with error: %ld\n", WSAGetLastError());
		DisplayInLogList(Log);
		closesocket(ListenSocket);
		return false;
	}

	//Syncs Socket To Event Message
	WSAAsyncSelect(ListenSocket, hWnd, wmEvent, FD_ACCEPT);

	// Change the socket mode on the listening socket from blocking to
	// non-block so the application will not block waiting for requests
	if (ioctlsocket(ListenSocket, FIONBIO, &BlockingMode) == SOCKET_ERROR)
	{
		wsprintf(Log, "ioctlsocket() failed with error %d\n", WSAGetLastError());
		wsprintf(Log, "Listen failed with error: %ld\n", WSAGetLastError());
		DisplayInLogList(Log);
		return false;
	}
	
	//Returns Socket To Set Socket
	return ListenSocket;
}

/**
* Accepts a client socket.
* This should be called by a server.
*
* @param ListenSocket a socket that has been created with socket(), bound to a local address with bind(), and is listening for connections after a listen().
* @param wmEvent event message for hWnd to handle the events on this socket
* @return the accepted socket
*/
SOCKET cSocket::AcceptSocket(SOCKET ListenSocket, unsigned int wmEvent)
{
	char Log[120];
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof (BOOL);
	SOCKET AcceptSocket;

	// Accept a client socket on listen socket
	AcceptSocket = accept(ListenSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET) {
		wsprintf(Log, "accept failed: %d\n", WSAGetLastError());
		DisplayInLogList(Log);
		closesocket(AcceptSocket);
		return 1;
	}

	//Syncs Socket To Event Message
	WSAAsyncSelect(AcceptSocket, hWnd, wmEvent, FD_READ | FD_WRITE | FD_CLOSE);

	//---------------------------------------
	// Initialize variables and call setsockopt. 
	// The SO_KEEPALIVE parameter is a socket option 
	// that makes the socket send keepalive messages
	// on the session. The SO_KEEPALIVE socket option
	// requires a boolean value to be passed to the
	// setsockopt function. If TRUE, the socket is
	// configured to send keepalive messages, if FALSE
	// the socket configured to NOT send keepalive messages.
	// This section of code tests the setsockopt function
	// by checking the status of SO_KEEPALIVE on the socket
	// using the getsockopt function.
	setsockopt(AcceptSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&bOptVal, 1);

	//Returns Socket To Set Socket
	return AcceptSocket;
}

/**
* Connects a server socket.
* This should be called by a client.
*
* @param ip server ip
* @param port server port
* @param wmEvent event message for hWnd to handle the events on this socket
* @return the connected socket
*/
SOCKET cSocket::ConnectSocket(char * ip, u_short port, unsigned int wmEvent)
{
	
	struct sockaddr_in saServer;
	int iResult;
	char Log[120];
	SOCKET ConnectSocket = NULL;

	//Creates socket to connect listen socket over network
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ConnectSocket == INVALID_SOCKET) {
		wsprintf(Log, "Error at socket(): %ld\n", WSAGetLastError());
		DisplayInLogList(Log);
		return 0;
	}

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
		DisplayInLogList(Log);
		closesocket(ConnectSocket);
		return 0;
	}

	//Syncs Socket To Event Message
	WSAAsyncSelect(ConnectSocket, hWnd, wmEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	//Returns Socket To Set Socket
	return ConnectSocket;
}

/**
* Sends data to a socket
*
* @param connectedSock the connected socket
* @param data A pointer to a buffer containing the data to be transmitted.
* @return the number of bytes sent
*/
/*
int cSocket::SendSocket(SOCKET connectedSock, char * data)
{
	char Log[120];

	int byteSent = send(connectedSock, data, strlen(data), 0 );
	if (byteSent == SOCKET_ERROR) {
		wsprintf(Log, "Send failed: %d\n", WSAGetLastError());
		DisplayInLogList(Log);
		closesocket(connectedSock);
		return 0;
	}
	return byteSent;
} */
int cSocket::SendSocket(SOCKET connectedSock, char * data, size_t Size)
{
	char Log[120];
	int total = 0, ret;        // how many bytes we've sent
	int bytesleft = Size; // how many we have left to send

	while (total < Size) {
		ret = send(connectedSock, data + total, bytesleft, 0);
		if (ret == SOCKET_ERROR) {
			wsprintf(Log, "Send failed: %d\n", WSAGetLastError());
			DisplayInLogList(Log);
			return 0;
			break;
		}
		total += ret;
		bytesleft -= ret;
	}
	Size = total; // return number actually sent here

	return ret == -1 ? -1 : 0; // return -1 onm failure, 0 on success
}

/**
* Receives data from a socket.
*
* @param acceptedSock the accepted socket
* @param buf A pointer to the buffer to receive the incoming data.
* @param len The length, in bytes, of the buffer pointed to by the buf parameter.
* @return the number of bytes received
*/
int cSocket::RecvSocket(SOCKET acceptedSock, char * buf, int len)
{
	char Log[120];

	int byteReceived = recv(acceptedSock, buf, len, 0 /* flags */);
	if (byteReceived == SOCKET_ERROR) {
		wsprintf(Log, "Receive failed: %d\n", WSAGetLastError());
		DisplayInLogList(Log);
		closesocket(acceptedSock);
		return 0;
	}
	return byteReceived;
}
