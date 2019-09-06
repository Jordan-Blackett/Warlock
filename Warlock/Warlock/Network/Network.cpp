#include "Network.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Network //////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Network::Network(MessagingSystem* messageBus)
	: BusNode(messageBus)
{
}

bool Network::Initialize()
{
	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;
	if (WSAStartup(winsockVersion, &winsockData))
	{
		std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		return false;
	}

	//std::shared_ptr<NewConnectionMessage> test = std::make_shared<NewConnectionMessage>(10);
	////std::shared_ptr<NewConnectionMessage> test(new NewConnectionMessage(10));
	////NewConnectionMessage *newConnectionMessage = new NewConnectionMessage(100);
	//SendMessageSystem(test);
	//Notify();

	printf("NETWORK: WSAStartup");
}

DWORD Network::TCPConsumer(void * sd_)
{
	int retval = 0;
	SOCKET sd = (SOCKET)sd_;

	if (!TCPReceive(sd)) {
		printf("NETWORK: Echo incoming packets failed");
		retval = 3;
	}

	//BT_CORE_WARN("NETWORK: Shutting connection down...");
	if (ShutdownConnection(sd) != SOCKET_ERROR) {
		printf("NETWORK: Connection is down");
	}
	else {
		printf("NETWORK: Connection shutdown failed");
		retval = 3;
	}

	return retval;
}

bool Network::TCPReceive(SOCKET sd)
{
	// Read data from client
	char acReadBuffer[BUFFER_SIZE];
	int readBytes;
	do {
		readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
		if (readBytes > 0) {
			printf("NETWORK: TCP - Received {0} bytes from client.", readBytes);
		}
		else if (readBytes == SOCKET_ERROR) {
			//return false;
		}
	} while (readBytes != 0);

	printf("Connection closed by peer.");
	return true;
}

void Network::TCP_Send(std::string Packet)
{
	//const int n = Packet.length();
	//char char_array[128];
	//strcpy_s(char_array, Packet.c_str());

	//send(clientSocket, char_array, n, 0);

	//send(clientSocket, Packet.c_str(), Packet.length(), 0);
}

SOCKET Network::CreateUDPSocket(const char * address, int port, sockaddr_in outhint)
{
	u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
	if (interfaceAddr != INADDR_NONE)
	{
		SOCKET UDPSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (UDPSocket != INVALID_SOCKET)
		{
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			hint.sin_addr.s_addr = interfaceAddr;
			hint.sin_port = htons(port);
			outhint = hint;

			if (bind(UDPSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
			{
				// Failed to bind socket
				//BT_CORE_ERROR("NETWORK: Unable To Bind UDP Socket.");
				// CleanUp();
				return INVALID_SOCKET;
			}
		}
		return UDPSocket;
	}
	return INVALID_SOCKET;
}

DWORD Network::UDPConsumer(SOCKET udpsocket, sockaddr_in hint)
{
	int retval = 0;

	if (!UDPReceive(udpsocket, hint)) {
		printf("NETWORK: Echo incoming packets failed");
		retval = 3;
	}

	printf("NETWORK: Shutting connection down...");
	if (ShutdownConnection(udpsocket) != SOCKET_ERROR) {
		printf("NETWORK: Connection is down");
	}
	else {
		printf("NETWORK: Connection shutdown failed");
		retval = 3;
	}

	return retval;
}

bool Network::UDPReceive(SOCKET udpsocket, sockaddr_in hint)
{
	SOCKADDR_IN client;
	ZeroMemory(&client, sizeof(client));
	int clientLength = sizeof(client);

	// Read data from client
	char acReadBuffer[BUFFER_SIZE];
	int flags = 0;

	int readBytes;
	do {
		readBytes = recvfrom(udpsocket, acReadBuffer, BUFFER_SIZE, flags, (SOCKADDR*)&client, &clientLength);
		if (readBytes > 0) {
			//BT_CORE_TRACE("NETWORK: UDP - Received {0} bytes from client.", readBytes);
		}
		else if (readBytes == SOCKET_ERROR) {
			return false;
		}
	} while (readBytes != 0);

	//BT_CORE_WARN("Connection closed by peer.");
	return true;
}

void Network::UDP_Send(std::string Packet)
{
	//int sendOk = sendto(UDPclientSocket, Packet.c_str(), Packet.length(), 0, (sockaddr*)&UDPhint, sizeof(UDPhint));
}

bool Network::ShutdownConnection(SOCKET sd)
{
	return closesocket(sd);
}

void Network::OnNotify(Message& message)
{
	message.GetCategoryFlags();
	message.GetMessageType();

	//int bufferOffset = 0;
	//uint16_t ClientID = 0;
	//uint16_t packetType = 0;
	//uint16_t packetSubType = 0;

	//// Client ID
	//memcpy(&ClientID, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//ClientID = ntohs(ClientID);
	//bufferOffset += sizeof(uint16_t);

	//// Type/ Sub-Type
	//memcpy(&packetType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//packetType = ntohs(packetType);
	//bufferOffset += sizeof(uint16_t);
	//memcpy(&packetSubType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//packetSubType = ntohs(packetSubType);
	//bufferOffset += sizeof(uint16_t);


	//// TODO: SKIPPED BY CASE LABEL
	//SnapshotPacket* snapshotPacket = new SnapshotPacket();
	//PlayerState* playerState = new PlayerState();
	//std::string newMessage("Snapshot::");

	//switch (packetType)
	//{
	//	// New_Connection
	//	case 0:
	//		std::cout << "Client ID: " << ClientID << std::endl;
	//		std::cout << "packetType: " << packetType << std::endl;
	//		clientID_ = ClientID;
	//	break;
	//	// Snapshot
	//	case 1:
	//		//std::cout << "Snapshot" << std::endl;

	//		// Snapshot

	//		//numOfPlayersStates
	//		memcpy(&snapshotPacket->numOfPlayersStates, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//		snapshotPacket->numOfPlayersStates = htons(snapshotPacket->numOfPlayersStates);
	//		bufferOffset += sizeof(uint16_t);

	//		// Player States
	//		for (int i = 0; i < snapshotPacket->numOfPlayersStates; i++)
	//		{
	//			memcpy(&playerState->playerID, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//			playerState->playerID = htons(playerState->playerID);
	//			bufferOffset += sizeof(uint16_t);
	//			memcpy(&playerState->positionX, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//			playerState->positionX = htons(playerState->positionX);
	//			bufferOffset += sizeof(uint16_t);
	//			memcpy(&playerState->positionY, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//			playerState->positionY = htons(playerState->positionY);
	//			bufferOffset += sizeof(uint16_t);
	//			memcpy(&playerState->angle, message.getMessage().c_str() + bufferOffset, sizeof(float));
	//			playerState->angle = htons(playerState->angle);
	//			bufferOffset += sizeof(float);
	//			memcpy(&playerState->health, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	//			playerState->health = htons(playerState->health);
	//			bufferOffset += sizeof(uint16_t);

	//			snapshotPacket->playerStates.push_back(playerState);
	//		}


	//		SendMessageSystem(Message(newMessage, snapshotPacket));

	//		//std::cout << playerState->positionX << "::" << playerState->positionY << std::endl;
	//		break;
	//	// Send Packet
	//	case 10: // Input
	//		//std::cout << "Packet Size: " << ClientID << std::endl;

	//		// Send
	//		TCP_Send(message.getMessage());
	//	break;
	//}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Server Network ///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ServerNetwork::ServerNetwork(MessagingSystem* messageBus)
	: Network(messageBus)
{
}

bool ServerNetwork::Initialize()
{
	Network::Initialize();

	TCPThread_ = std::thread(&ServerNetwork::InitTCP, this, IP, PORT);
	//UDPThread_ = std::thread(&ServerNetwork::InitUDP, this, IP, PORT);

	return true;
}

void ServerNetwork::InitTCP(const char* address, int port)
{
	printf("NETWORK: Establishing TCP Listener...\n");
	SOCKET listeningSocket = CreateListenSocket(address, port);
	if (listeningSocket == INVALID_SOCKET)
	{
		printf("NETWORK: ListeningSocket - INVALID_SOCKET.\n");
		system("pause");
		//return false;
	}
	printf("NETWORK: TCP Listener Established.\n");

	printf("NETWORK: Waiting for connections...\n");
	while (1) {
		AcceptConnections(listeningSocket);
		printf("Acceptor restarting...\n");
	}
}

void ServerNetwork::InitUDP(const char * address, int port)
{
	printf("NETWORK: Establishing UDP Socket...\n");
	sockaddr_in hint = sockaddr_in();
	SOCKET UDPSocket = CreateUDPSocket(address, port, hint);
	if (UDPSocket == INVALID_SOCKET)
	{
		printf("NETWORK: UDPSocket - INVALID_SOCKET.\n");
		system("pause");
	}

	printf("NETWORK: UDP Established.\n");
	//UDPConsumer(UDPSocket, hint);
}

SOCKET ServerNetwork::CreateListenSocket(const char * address, int port)
{
	u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
	if (interfaceAddr != INADDR_NONE)
	{
		SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listeningSocket != INVALID_SOCKET)
		{
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			//hint.sin_addr.s_addr = interfaceAddr;
			hint.sin_addr.s_addr = INADDR_ANY;
			hint.sin_port = htons(port);

			if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
			{
				if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
				{
					return INVALID_SOCKET;
				}
			}
			else
			{
				// Failed to bind socket
				printf("NETWORK: Unable To Bind Listen Socket.\n");
				//CleanUp();
				return INVALID_SOCKET;
			}
			return listeningSocket;
		}
	}
	return INVALID_SOCKET;
}

void ServerNetwork::AcceptConnections(SOCKET ListeningSocket)
{
	sockaddr_in sinRemote;
	int addrSize = sizeof(sinRemote);

	while (true) {
		SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote, &addrSize);
		//SOCKET sd = accept(ListeningSocket, NULL, NULL);
		if (sd != INVALID_SOCKET) {
			//printf("NETWORK: Accepted Connection From {0} : {1}\n", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));
			std::cout << "New connection: " << sd << std::endl;

			ConnectionReceived(sd);

			DWORD threadID;
			CreateThread(0, 0, TCPConsumer, (void*)sd, 0, &threadID);
		}
		else {
			//BT_CORE_WARN("NETWORK: Accept() Failed.");
			//std::cout << "Connection failed: " << newClientSocket << std::endl;
			return;
		}
	}
}

void ServerNetwork::ConnectionReceived(SOCKET newSocket)
{
	m_Connections.insert(std::pair<unsigned int, SOCKET>(newSocket, newSocket));

	// Send new connection ID to game
	//NewConnectionMessage newConnectionMessage(newSocket);
	SendMessageSystem(std::make_shared<NewConnectionMessage>(newSocket));
	//std::shared_ptr<NewConnectionMessage> newConnectionMessage = std::make_shared<NewConnectionMessage>(newSocket);
	//SendMessageSystem(newConnectionMessage);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Client Network ///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClientNetwork::ClientNetwork(MessagingSystem * messageBus) 
	: Network(messageBus)
{
}

bool ClientNetwork::Initialize()
{
	Network::Initialize();

	TCPThread_ = std::thread(&ClientNetwork::InitTCP, this, IP, PORT);
	//UDPThread_ = std::thread(&ClientNetwork::InitUDP, this, IP, PORT);

	return false;
}

void ClientNetwork::InitTCP(const char* address, int port)
{
	printf("NETWORK: Establishing TCP Connection\n");
	SOCKET listeningSocket = CreateTCPSocket(address, port);
	if (listeningSocket == INVALID_SOCKET)
	{
		printf("NETWORK: ListeningSocket - INVALID_SOCKET.\n");
		system("pause");
		//return false;
	}

	while (1) {
		printf("NETWORK: TCPReceive.\n");
		TCPReceive(listeningSocket);
	}

//char buf[MAX_BUFFER_SIZE];

//int pbytesReceived = 0;
//int pincomingPacketLength = 2;

//// TCP Receive
//while (true)
//{
//	ZeroMemory(buf, MAX_BUFFER_SIZE);

//	//int bytesReceived = recv(clientSocket, buf, 4096, 0);
//	//if (bytesReceived > 0)
//	//{
//	//	// Send packet to messaging system
//	//	Message packet(std::string(buf, bytesReceived));
//	//	SendMessageSystem(packet);
//	//}

//	int bytesReceived = recv(clientSocket, buf, pincomingPacketLength - pbytesReceived, 0);
//	if (bytesReceived > 0)
//	{
//		if (pincomingPacketLength == 2)
//		{
//			// Get packet length value of first 2 bytes
//			uint16_t packetLength;
//			memcpy(&packetLength, buf, 2);
//			packetLength = htons(packetLength);
//			pincomingPacketLength = packetLength;
//		}

//		memcpy(buf + pbytesReceived, buf, bytesReceived);

//		pbytesReceived += bytesReceived;
//		std::cout << bytesReceived << " :: " << pbytesReceived << std::endl;
//		if (pbytesReceived >= pincomingPacketLength)
//		{
//			int headerSize = 6;

//			// Send packet to messaging system
//			Message packet(std::string(buf, pbytesReceived));
//			SendMessageSystem(packet);

//			pbytesReceived = 0;
//			pincomingPacketLength = 2; // 2 = Packet length size (bytes)
//			ZeroMemory(buf, MAX_BUFFER_SIZE);
//		}
//	}
//}
}

void ClientNetwork::InitUDP(const char * address, int port)
{
	//// Create a hint structure for the server //UDP
	////sockaddr_in hint;
	//UDPhint.sin_family = AF_INET; // AF_INET = IPv4 addresses
	//UDPhint.sin_port = htons(UDP_PORT); // Little to big endian conversion
	////inet_pton(AF_INET, ipAddress.c_str(), &UDPhint.sin_addr); // Convert from string to byte array
	//UDPhint.sin_addr.s_addr = INADDR_ANY;

	//UDPclientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	//if (UDPclientSocket == INVALID_SOCKET)
	//{
	//	printf("socket failed: %d", WSAGetLastError());
	//	WSACleanup();
	//	//return 1;
	//}

	//// Try and bind the socket to the IP and port
	//if (bind(UDPclientSocket, (SOCKADDR*)&UDPhint, sizeof(UDPhint)) == SOCKET_ERROR)
	//{
	//	std::cout << "udp error" << std::endl;
	//}
	//
	//char buf[MAX_BUFFER_SIZE];

	//char ipstr[INET_ADDRSTRLEN];
	//void* addr = &UDPhint.sin_addr;
	//InetNtop(UDPhint.sin_family, addr, (PSTR)ipstr, sizeof(ipstr));
	//std::cout << "initUDP [" << ipstr << "]:: " << ntohs(UDPhint.sin_port) << std::endl;

	//int clientLength = sizeof(UDPhint);

	//char buffer[1024]; //SOCKET_BUFFER_SIZE
	//int flags = 0;

	//// UDP Receive
	//while (true)
	//{
	//	ZeroMemory(&buffer, sizeof(buffer));

	//	int bytes_received = recvfrom(UDPclientSocket, buffer, sizeof(buffer), flags, (SOCKADDR*)&UDPhint, &clientLength);
	//	if (bytes_received == SOCKET_ERROR)
	//	{
	//		//printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
	//		continue;
	//	}

	//	//std::cout << "test2" << std::endl;

	//	uint16_t packetLength;
	//	memcpy(&packetLength, buffer, 2);
	//	packetLength = htons(packetLength);

	//	// Send packet to messaging system
	//	Message packet(std::string(buffer, packetLength));
	//	SendMessageSystem(packet);


	//	////int bytesReceived = recv(UDPclientSocket, buf, 4096, 0);
	//	//int bytesReceived = recvfrom(UDPclientSocket, buf, sizeof(buf), 0, (SOCKADDR*)&UDPhint, &clientLength);
	//	//if (bytesReceived > 0)
	//	//{
	//	//	// Send packet to messaging system
	//	//	Message packet(std::string(buf, bytesReceived));
	//	//	SendMessageSystem(packet);
	//	//}
	//}

	//closesocket(UDPclientSocket);
}

SOCKET ClientNetwork::CreateTCPSocket(const char * address, int port)
{
	u_long interfaceAddr = InetPton(AF_INET, address, &interfaceAddr);
	if (interfaceAddr != INADDR_NONE)
	{
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket != INVALID_SOCKET)
		{
			sockaddr_in hint;
			hint.sin_family = AF_INET;
			//hint.sin_addr.s_addr = interfaceAddr;
			InetPton(AF_INET, "127.0.0.1", &hint.sin_addr.s_addr);
			hint.sin_port = htons(port);

			if (connect(clientSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
			{
				printf("connect to the server: %d", WSAGetLastError());
			}
			else
			{
				// Failed to connect to server
				printf("Failed to connect to the server: %d", WSAGetLastError());
				closesocket(clientSocket);
				//Cleanup();
				return INVALID_SOCKET;
			}
		}
		return clientSocket;
	}
	return INVALID_SOCKET;
}
