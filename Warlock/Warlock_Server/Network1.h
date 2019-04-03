#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "TCPListener.h"
#include "TCPSocket.h"
#include "UDPNetwork.h"
#include <functional>

#include "BusNode.h"
#include <thread>

#define TCP_PORT 54000
#define UDP_PORT 52000
#define MAX_BUFFER_SIZE (25) //49152

class Network1 : public BusNode
{
public:
	Network1(MessagingSystem* messageBus);
	~Network1();

	bool Init();
	void InitListener();
	void InitUDP();

	void SendToMessageSystem(const std::string* msg);

	void run();

	void CleanUp();

	void Listener_ConnectionReceived(SOCKET* socket);

	void Send(int clientSocket, std::string msg);

private:
	void PrintExceptionalCondition(SOCKET socket);

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	std::thread TCPlistenerThread_;
	std::thread UDPReceiveThread_;

	std::vector<TCPSocket> connections_;

	void onNotify(Message message) {}
};

