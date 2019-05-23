#include "Network1.h"



Network1::Network1(MessagingSystem* messageBus) : BusNode(messageBus)
{
}


Network1::~Network1()
{
}

bool Network1::Init()
{
	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;
	if (WSAStartup(winsockVersion, &winsockData))
	{
		std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		system("pause");
		return false;
	}

	TCPlistenerThread_ = std::thread(&Network1::InitListener, this);
	UDPReceiveThread_ = std::thread(&Network1::InitUDP, this);

	return true;
}

void Network1::InitListener()
{
	// Create a listener socket and make it wait for new connections
	using namespace std::placeholders;
	TCPListener listener("127.0.0.1", TCP_PORT, std::bind(&Network1::Listener_ConnectionReceived, this, _1));
	if (listener.Init())
	{
		listener.listen();
	}
	// else error return -1 break etc etc
}

void Network1::InitUDP()
{
	using namespace std::placeholders;
	UDPNetwork UDP(UDP_PORT, std::bind(&Network1::SendToMessageSystem, this, _1));
	if (UDP.Init())
	{
		UDP.Run();
	}
}

std::string Network1::CreatePacket(uint16_t clientID, uint16_t packetType, uint16_t packetSubType, std::string message)
{
	char packetBuffer[128];

	uint16_t clientIDHtoN = htons(clientID);
	uint16_t packetTypeHtoN = htons(packetType);
	uint16_t packetSubTypeHtoN = htons(packetSubType);

	// Header
	memcpy(packetBuffer, &clientIDHtoN, sizeof(uint16_t));
	memcpy(packetBuffer + 2, &packetTypeHtoN, sizeof(uint16_t));
	memcpy(packetBuffer + 4, &packetSubTypeHtoN, sizeof(uint16_t));

	// Message
	if (message != "")
	{
		strcpy_s(packetBuffer + 6, sizeof(packetBuffer), message.c_str());
	}

	return std::string(packetBuffer, 6);
}

std::string Network1::CreatePacket2(uint16_t type, uint16_t sub, PacketData* data)
{
	buffer_[128]; // TODO: clear
	bufferSize_ = 0;
	bufferOffset_ = 2;

	PacketHeader* packetHeader = new PacketHeader();
	packetHeader->msgType = type;
	packetHeader->msgSubType = sub;

	htonHeaderData(*packetHeader, *data);

	bufferSize_ = bufferOffset_;

	return std::string(buffer_, bufferSize_);
}

void Network1::htonHeaderData(const PacketHeader & header, const PacketData & data)
{
	htonHeader(header);
	htonData(data);

	// Packet size
	uint16_t u16;
	u16 = htons(bufferOffset_);
	memcpy(buffer_, &u16, sizeof(uint16_t));
}

void Network1::htonHeader(const PacketHeader & header)
{
	uint16_t u16;
	u16 = htons(header.msgType);
	memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
	bufferOffset_ += sizeof(uint16_t);
	u16 = htons(header.msgSubType);
	memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
	bufferOffset_ += sizeof(uint16_t);
}

void Network1::htonData(const PacketData & data)
{
	if (data.snapshot != nullptr)
	{
		for(auto const& obj : data.snapshot->objectStates)
		{
			uint16_t u16;
			u16 = htons(obj->positionX);
			memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
			bufferOffset_ += sizeof(uint16_t);
			u16 = htons(obj->positionY);
			memcpy(buffer_ + bufferOffset_, &u16, sizeof(uint16_t));
			bufferOffset_ += sizeof(uint16_t);
		}
	}
}

void Network1::SendToMessageSystem(const std::string * msg)
{
	Message packet(*msg);
	SendMessageSystem(packet);
}

void Network1::run()
{
	while (true)
	{
		// Message System
		//Notify();

		if (connections_.size() == 0)
		{
			continue;
		}

		FD_ZERO(&readfds_);
		//FD_ZERO(&writefds_);
		FD_ZERO(&exceptfds_);

		// Recieve
		for (auto const& conn : connections_)
		{
			FD_SET(conn.second.socket_, &readfds_);
			FD_SET(conn.second.socket_, &exceptfds_);
		}

		select(0, &readfds_, &writefds_, &exceptfds_, NULL);

		char buf[MAX_BUFFER_SIZE];

		for (auto& conn : connections_)
		{
			if (FD_ISSET(conn.second.socket_, &exceptfds_))
			{
				PrintExceptionalCondition(conn.second.socket_);
				std::cout << "Winsock error occurred on client:" << conn.second.socket_ << ". Disconnecting client." << std::endl;
				closesocket(conn.second.socket_);
				connections_.erase(conn.first);
				continue;
			}

			if (FD_ISSET(conn.second.socket_, &readfds_))
			{
				ZeroMemory(buf, MAX_BUFFER_SIZE);

				int bytesReceived = recv(conn.second.socket_, buf, conn.second.incomingPacketLength - conn.second.bytesReceived, 0);
				if (bytesReceived > 0)
				{
					if (conn.second.incomingPacketLength == 2)
					{
						// Get packet length value of first 2 bytes
						uint16_t packetLength;
						memcpy(&packetLength, buf, 2);
						packetLength = htons(packetLength);
						conn.second.incomingPacketLength = packetLength;
					}

					memcpy(conn.second.buffer + conn.second.bytesReceived, buf, bytesReceived);

					conn.second.bytesReceived += bytesReceived;
					if (conn.second.bytesReceived >= conn.second.incomingPacketLength)
					{
						int headerSize = 6;

						// Client ID - Swap packet length to client ID
						uint16_t u16;
						u16 = htons(conn.second.socket_);
						memcpy(conn.second.buffer, &u16, sizeof(uint16_t));

						// Send packet to messaging system
						Message packet(std::string(conn.second.buffer, conn.second.incomingPacketLength));
						SendMessageSystem(packet);

						conn.second.bytesReceived = 0;
						conn.second.incomingPacketLength = 2; // 2 = Packet length size (bytes)
						ZeroMemory(conn.second.buffer, MAX_BUFFER_SIZE);
					}
				}
				else if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						std::cout << "Lost connection to " << conn.second.socket_ << std::endl;
						closesocket(conn.second.socket_);
						connections_.erase(conn.first);
						continue;
					}
				}
			}
		}
		// Message System
		Notify();
	}
	CleanUp();
}

void Network1::CleanUp()
{
	for (auto& conn : connections_)
	{
		closesocket(conn.second.socket_);
		connections_.erase(conn.first);
	}
}

void Network1::Listener_ConnectionReceived(SOCKET* socket)
{
	connections_.insert(std::pair<unsigned int, TCPSocket>(*socket, *socket));

	// Send new connection ID to game
	std::string connectionPacket = CreatePacket(*socket, 0, 0, ""); //TODO:
	Message packet(connectionPacket);
	SendMessageSystem(packet);
	Notify();

	// Send new connection ID to client
	// loop through clients TODO:
	// TODO: UDP SEND TCP SEND
	//Send(connections_[*socket].socket_, connectionPacket); //UDP
}

void Network1::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

void Network1::SendAll(std::string msg)
{
	for (auto const& ID : connections_)
	{
		send(ID.first, msg.c_str(), msg.size() + 1, 0);
	}
}

void Network1::PrintExceptionalCondition(SOCKET socket)
{
	int errorCode = 0;
	int errorCodeSize = sizeof(errorCode);
	if (getsockopt(socket, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &errorCodeSize) == 0)
	{
		std::cout << "Winsock error: " << errorCode << "." << std::endl;
	}
	else
	{
		std::cout << "Winsock error occurred. getsockopt() failed." << std::endl;
	}
}
