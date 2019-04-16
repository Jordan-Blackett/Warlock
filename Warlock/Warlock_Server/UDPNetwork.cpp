#include "UDPNetwork.h"



UDPNetwork::UDPNetwork(int port, std::function<void(const std::string*)> handler) : port_(port), SendMessageSystem(handler)
{
}


UDPNetwork::~UDPNetwork()
{
}

bool UDPNetwork::Init()
{
	// Create a socket, notice that it is a user datagram socket (UDP)
	UDPSocket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Create a server hint structure for the server
	SOCKADDR_IN UDPHint;
	UDPHint.sin_family = AF_INET;
	UDPHint.sin_port = htons(port_);
	UDPHint.sin_addr.s_addr = INADDR_ANY;

	// Try and bind the socket to the IP and port
	if (bind(UDPSocket_, (SOCKADDR*)&UDPHint, sizeof(UDPHint)) == SOCKET_ERROR)
	{
		//printf("bind failed: %d", WSAGetLastError());
		return false;
	}

	//std::cout << "UDP Successful." << std::endl;
	return true;
}

void UDPNetwork::Run() //UDP recv
{
	SOCKADDR_IN client;
	ZeroMemory(&client, sizeof(client));
	int clientLength = sizeof(client);

	char buffer[1024]; //SOCKET_BUFFER_SIZE
	int flags = 0;

	while (true)
	{
		ZeroMemory(&buffer, sizeof(buffer));

		int bytes_received = recvfrom(UDPSocket_, buffer, sizeof(buffer), flags, (SOCKADDR*)&client, &clientLength);
		if (bytes_received == SOCKET_ERROR)
		{
			//printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
			continue;
		}

		char clientIP[256];
		ZeroMemory(clientIP, 256);

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

		std::string test(buffer, 1024);
		SendMessageSystem(&test);

		// Display the message / who sent it
		//std::cout << "Message recv from " << clientIP << " : " << buffer << std::endl;
	}

	closesocket(UDPSocket_);
}
