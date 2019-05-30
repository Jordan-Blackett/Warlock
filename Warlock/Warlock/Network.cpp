#include "Network.h"



Network::Network(Client_MessagingSystem* messageBus) : Client_BusNode(messageBus)
{
}


Network::~Network()
{
}

bool Network::Init()
{
	std::string ipAddress = "127.0.0.1";
	std::cout << "Network Init - IP: " << ipAddress << " Port: " << TCP_PORT << std::endl;

	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;
	if (WSAStartup(winsockVersion, &winsockData))
	{
		std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		system("pause");
		return false;
	}

	TCPThread_ = std::thread(&Network::InitTCP, this);
	UDPThread_ = std::thread(&Network::InitUDP, this);

	return true;
}

void Network::InitTCP()
{
	// Create a socket
	int addressFamily = AF_INET;
	int type = SOCK_STREAM; // TCP
	int protocol = IPPROTO_TCP;

	clientSocket = socket(addressFamily, type, protocol);
	if (clientSocket == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		WSACleanup();
		//return 1;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(TCP_PORT);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	if (connect(clientSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		printf("Failed to connect to the server: %d", WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
		//return 1;
	}

	char buf[MAX_BUFFER_SIZE];

	int pbytesReceived = 0;
	int pincomingPacketLength = 2;

	// TCP Receive
	while (true)
	{
		ZeroMemory(buf, MAX_BUFFER_SIZE);

		//int bytesReceived = recv(clientSocket, buf, 4096, 0);
		//if (bytesReceived > 0)
		//{
		//	// Send packet to messaging system
		//	Message packet(std::string(buf, bytesReceived));
		//	SendMessageSystem(packet);
		//}

		int bytesReceived = recv(clientSocket, buf, pincomingPacketLength - pbytesReceived, 0);
		if (bytesReceived > 0)
		{
			if (pincomingPacketLength == 2)
			{
				// Get packet length value of first 2 bytes
				uint16_t packetLength;
				memcpy(&packetLength, buf, 2);
				packetLength = htons(packetLength);
				pincomingPacketLength = packetLength;
			}

			memcpy(buf + pbytesReceived, buf, bytesReceived);

			pbytesReceived += bytesReceived;
			std::cout << bytesReceived << " :: " << pbytesReceived << std::endl;
			if (pbytesReceived >= pincomingPacketLength)
			{
				int headerSize = 6;

				// Send packet to messaging system
				Message packet(std::string(buf, pbytesReceived));
				SendMessageSystem(packet);

				pbytesReceived = 0;
				pincomingPacketLength = 2; // 2 = Packet length size (bytes)
				ZeroMemory(buf, MAX_BUFFER_SIZE);
			}
		}
	}
}

void Network::InitUDP()
{
	// Create a hint structure for the server //UDP
	//sockaddr_in hint;
	UDPhint.sin_family = AF_INET; // AF_INET = IPv4 addresses
	UDPhint.sin_port = htons(UDP_PORT); // Little to big endian conversion
	//inet_pton(AF_INET, ipAddress.c_str(), &UDPhint.sin_addr); // Convert from string to byte array
	UDPhint.sin_addr.s_addr = INADDR_ANY;

	UDPclientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDPclientSocket == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		WSACleanup();
		//return 1;
	}

	// Try and bind the socket to the IP and port
	if (bind(UDPclientSocket, (SOCKADDR*)&UDPhint, sizeof(UDPhint)) == SOCKET_ERROR)
	{
		std::cout << "udp error" << std::endl;
	}
	
	char buf[MAX_BUFFER_SIZE];

	char ipstr[INET_ADDRSTRLEN];
	void* addr = &UDPhint.sin_addr;
	InetNtop(UDPhint.sin_family, addr, (PSTR)ipstr, sizeof(ipstr));
	std::cout << "initUDP [" << ipstr << "]:: " << ntohs(UDPhint.sin_port) << std::endl;

	int clientLength = sizeof(UDPhint);

	char buffer[1024]; //SOCKET_BUFFER_SIZE
	int flags = 0;

	// UDP Receive
	while (true)
	{
		ZeroMemory(&buffer, sizeof(buffer));

		int bytes_received = recvfrom(UDPclientSocket, buffer, sizeof(buffer), flags, (SOCKADDR*)&UDPhint, &clientLength);
		if (bytes_received == SOCKET_ERROR)
		{
			//printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
			continue;
		}

		//std::cout << "test2" << std::endl;

		uint16_t packetLength;
		memcpy(&packetLength, buffer, 2);
		packetLength = htons(packetLength);

		// Send packet to messaging system
		Message packet(std::string(buffer, packetLength));
		SendMessageSystem(packet);


		////int bytesReceived = recv(UDPclientSocket, buf, 4096, 0);
		//int bytesReceived = recvfrom(UDPclientSocket, buf, sizeof(buf), 0, (SOCKADDR*)&UDPhint, &clientLength);
		//if (bytesReceived > 0)
		//{
		//	// Send packet to messaging system
		//	Message packet(std::string(buf, bytesReceived));
		//	SendMessageSystem(packet);
		//}
	}

	closesocket(UDPclientSocket);
}

void Network::TCP_Send(std::string Packet)
{
	//const int n = Packet.length();
	//char char_array[128];
	//strcpy_s(char_array, Packet.c_str());

	//send(clientSocket, char_array, n, 0);

	send(clientSocket, Packet.c_str(), Packet.length(), 0);
}

void Network::UDP_Send(std::string Packet)
{
	int sendOk = sendto(UDPclientSocket, Packet.c_str(), Packet.length(), 0, (sockaddr*)&UDPhint, sizeof(UDPhint));
}

void Network::onNotify(Message message)
{
	int bufferOffset = 0;
	uint16_t ClientID = 0;
	uint16_t packetType = 0;
	uint16_t packetSubType = 0;

	// Client ID
	memcpy(&ClientID, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	ClientID = ntohs(ClientID);
	bufferOffset += sizeof(uint16_t);

	// Type/ Sub-Type
	memcpy(&packetType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	packetType = ntohs(packetType);
	bufferOffset += sizeof(uint16_t);
	memcpy(&packetSubType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	packetSubType = ntohs(packetSubType);
	bufferOffset += sizeof(uint16_t);


	// TODO: SKIPPED BY CASE LABEL
	SnapshotPacket* snapshotPacket = new SnapshotPacket();
	PlayerState* playerState = new PlayerState();
	std::string newMessage("Snapshot::");

	switch (packetType)
	{
		// New_Connection
		case 0:
			std::cout << "Client ID: " << ClientID << std::endl;
			std::cout << "packetType: " << packetType << std::endl;
			clientID_ = ClientID;
		break;
		// Snapshot
		case 1:
			//std::cout << "Snapshot" << std::endl;

			// Snapshot

			memcpy(&playerState->positionX, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
			playerState->positionX = htons(playerState->positionX);
			bufferOffset += sizeof(uint16_t);
			memcpy(&playerState->positionY, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
			playerState->positionY = htons(playerState->positionY);
			bufferOffset += sizeof(uint16_t);
			memcpy(&playerState->angle, message.getMessage().c_str() + bufferOffset, sizeof(float));
			playerState->angle = htons(playerState->angle);
			bufferOffset += sizeof(float);
			memcpy(&playerState->health, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
			playerState->health = htons(playerState->health);
			bufferOffset += sizeof(uint16_t);

			snapshotPacket->playerStates.push_back(playerState);

			SendMessageSystem(Message(newMessage, snapshotPacket));

			//std::cout << playerState->positionX << "::" << playerState->positionY << std::endl;
			break;
		// Send Packet
		case 10: // Input
			//std::cout << "Packet Size: " << ClientID << std::endl;

			// Send
			TCP_Send(message.getMessage());
		break;
	}
}

