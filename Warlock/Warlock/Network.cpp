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

	SOCKET clientSocket = socket(addressFamily, type, protocol);
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

	// TCP Receive
	while (true)
	{
		ZeroMemory(buf, MAX_BUFFER_SIZE);

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			// Send packet to messaging system
			Message packet(std::string(buf, bytesReceived));
			SendMessageSystem(packet);
		}
	}
}

void Network::InitUDP()
{
	// Create a hint structure for the server //UDP
	sockaddr_in hint;
	hint.sin_family = AF_INET; // AF_INET = IPv4 addresses
	hint.sin_port = htons(UDP_PORT); // Little to big endian conversion
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr); // Convert from string to byte array

	SOCKET UDPclientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDPclientSocket == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		WSACleanup();
		//return 1;
	}
	
	char buf[MAX_BUFFER_SIZE];

	// UDP Receive
	while (true)
	{
		ZeroMemory(buf, MAX_BUFFER_SIZE);

		int bytesReceived = recv(UDPclientSocket, buf, 4096, 0);
		if (bytesReceived > 0)
		{
			// Send packet to messaging system
			Message packet(std::string(buf, bytesReceived));
			SendMessageSystem(packet);
		}
	}
}

void Network::TCP_Send(std::string Packet)
{
}

void Network::UDP_Send(std::string Packet)
{
}
