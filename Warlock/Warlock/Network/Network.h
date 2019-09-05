#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <thread>

#include "../MessagingSystem/BusNode.h"

//#include <iostream>
//#include <string>
//#include <vector>
//#include <map>
//#include "TestPacket.h"
//#include "Client_SnapshotPacket.h"

#define TCP_PORT 54000
#define UDP_PORT 8000
#define MAX_BUFFER_SIZE (25) //49152

#define PORT 5555
#define BUFFER_SIZE 1024

#define IP "127.0.0.1"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

class Network : public BusNode
{
public:
	Network(MessagingSystem* messageBus);
	~Network() {};

	virtual bool Initialize() = 0;
	virtual void InitTCP(const char* address, int port) = 0;
	virtual void InitUDP(const char* address, int port) = 0;

	// TCP

	static DWORD TCPConsumer(void* sd_);
	static bool TCPReceive(SOCKET sd);

	void TCP_Send(std::string Packet);

	// UDP

	SOCKET CreateUDPSocket(const char* address, int port, sockaddr_in outhint);
	DWORD UDPConsumer(SOCKET udpsocket, sockaddr_in hint);
	bool UDPReceive(SOCKET udpsocket, sockaddr_in hint);

	void UDP_Send(std::string Packet);

	// Helper Functions

	static bool ShutdownConnection(SOCKET sd);

protected:
	void OnNotify(Message& message);

	//SOCKET clientSocket;
	//sockaddr_in TCPhint;
	//SOCKET UDPclientSocket;
	//sockaddr_in UDPhint;

	std::thread TCPThread_;
	std::thread UDPThread_;

	//std::string ipAddress = "127.0.0.1";

	//u_int64 clientID_;
};

class ServerNetwork : public Network
{
public:
	ServerNetwork(MessagingSystem* messageBus);

	virtual bool Initialize() override;
	virtual void InitTCP(const char* address, int port) override;
	virtual void InitUDP(const char* address, int port) override;

	SOCKET CreateListenSocket(const char* address, int port);
	void AcceptConnections(SOCKET ListeningSocket);

private:
	//std::map<u_int64, TCPSocket> m_Connections;
	//std::map<u_int64, SOCKADDR_IN> m_ConnectionsUDP;
};

class ClientNetwork : public Network
{
public:
	virtual void InitTCP(const char* address, int port) override;
	virtual void InitUDP(const char* address, int port) override;

	SOCKET CreateTCPSocket(const char* address, int port);
private:

};
