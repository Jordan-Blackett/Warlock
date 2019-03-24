#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "TCPListener.h"
#include "TCPSocket.h"
#include <functional>

#define PORT 54000
#define MAX_BUFFER_SIZE (49152)

//void Listener_MessageReceived(TCPListener* listener, int client, std::string msg);
//void Listener_ConnectionReceived(SOCKET* socket);

class Network1
{
public:
	Network1();
	~Network1();

	bool Init();

	void run();

	void CleanUp();

	void Listener_ConnectionReceived(SOCKET* socket);

	void Send(int clientSocket, std::string msg);

	// 


private:
	//std::string ipAddress_;
	//int port_;
	//ConnectionRecievedHandler MessageRecieved;


	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	std::vector<TCPSocket> connections_;

	//TCPListener listener = TCPListener("127.0.0.1", PORT, std::bind(&Network1::Listener_ConnectionReceived, this, _1));
	TCPListener* listener;
};

