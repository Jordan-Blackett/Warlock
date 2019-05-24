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
#include <map>

#define TCP_PORT 54000
#define UDP_PORT 52000
#define MAX_BUFFER_SIZE (25) //49152

struct PacketHeader {
	uint16_t msgType;
	uint16_t msgSubType;
};

struct PacketData {
	SnapshotPacket* snapshot;
};

class Network1 : public BusNode
{
public:
	Network1(MessagingSystem* messageBus);
	~Network1();

	bool Init();
	void InitListener();
	void InitUDP();

	std::string CreatePacket(uint16_t clientid, uint16_t type, uint16_t sub, std::string message);
	std::string CreatePacket2(uint16_t type, uint16_t sub, PacketData* data);

	void htonHeaderData(const struct PacketHeader& header, const struct PacketData& data);
	void htonHeader(const struct PacketHeader& header);
	void htonData(const struct PacketData& data);

	void SendToMessageSystem(const std::string* msg);

	void run();

	void CleanUp();

	void Listener_ConnectionReceived(SOCKET* socket);
	
	void Send(int clientSocket, std::string msg);
	void SendAll(std::string msg);

private:
	void PrintExceptionalCondition(SOCKET socket);

	fd_set readfds_;
	fd_set writefds_;
	fd_set exceptfds_;

	std::thread TCPlistenerThread_;
	std::thread UDPReceiveThread_;

	std::map<u_int64, TCPSocket> connections_;
	//std::vector<TCPSocket> connections_;

	// Packet Data
	char buffer_[128];
	int bufferSize_ = 0;
	int bufferOffset_ = 2; // 0-1 is packet size

	void onNotify(Message message) 
	{
		if (message.getMessage() == "Snapshot::")
		{
			//std::cout << message.getMessage() << "::" << message.GetSnapshotPacket() << std::endl;

			PacketData* data = new PacketData();
			data->snapshot = message.GetSnapshotPacket();
			SendAll(CreatePacket2(1, 0, data));
		}
	}
};

