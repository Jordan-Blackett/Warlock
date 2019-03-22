#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

#include "TCPSocket.h"

#define PORT 54000
#define MAX_BUFFER_SIZE (49152)

class TCPListener;
typedef void(*MessageRecievedHandler)(TCPListener* listener, int socketID, std::string msg);

class TCPListener
{
public:
	TCPListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	~TCPListener();

	bool Init();

	void listen();

	void CleanUp();

	void Send(int clientSocket, std::string msg);

private:
	SOCKET CreateListenSocket();
	SOCKET WaitForConnection(SOCKET listeningSocket);

	void PrintExceptionalCondition(SOCKET socket);

	std::string ipAddress_;
	int port_;
	MessageRecievedHandler MessageRecieved;

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	std::vector<TCPSocket> connections_;

};
