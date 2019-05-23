#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <thread>
#include "Client_BusNode.h"

#include "TestPacket.h"
#include "Client_SnapshotPacket.h"

#define TCP_PORT 54000
#define UDP_PORT 52000
#define MAX_BUFFER_SIZE (25) //49152

class Network : public Client_BusNode
{
public:
	Network(Client_MessagingSystem* messageBus);
	~Network();

	bool Init();
	void InitTCP();
	void InitUDP();

	void TCP_Send(std::string Packet);
	void UDP_Send(std::string Packet);

private:
	void onNotify(Message message);

	SOCKET clientSocket;
	sockaddr_in TCPhint;
	SOCKET UDPclientSocket;
	sockaddr_in UDPhint;

	std::thread TCPThread_;
	std::thread UDPThread_;

	std::string ipAddress = "127.0.0.1";

	u_int64 clientID_;

};

