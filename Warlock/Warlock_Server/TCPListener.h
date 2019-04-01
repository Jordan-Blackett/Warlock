#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <functional>

class TCPListener
{
public:
	//TCPListener();
	TCPListener(std::string ipAddress, int port, std::function<void(SOCKET*)> handler);
	~TCPListener();

	bool Init();

	void listen();

	void CleanUp();

private:
	SOCKET CreateListenSocket();
	SOCKET WaitForConnection(SOCKET listeningSocket);

	void PrintExceptionalCondition(SOCKET socket);

	SOCKET listeningSocket;

	std::string ipAddress_;
	int port_;

	std::function<void(SOCKET*)> connectionRecieved_;

	fd_set readfds_;
	fd_set exceptfds_;
};

