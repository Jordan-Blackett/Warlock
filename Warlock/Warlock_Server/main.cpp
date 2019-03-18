#include <iostream>

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "ws2_32.lib")

#define PORT 54000
#define SOCKET_BUFFER_SIZE 4096

int main() {
	std::cout << "Hello World Server" << std::endl;

	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;

	if (WSAStartup(winsockVersion, &winsockData))
	{
		printf("WSAStartup failed: %d", WSAGetLastError());
		return 1;
	}

	// Create a socket - TCP
	int addressFamily = AF_INET;
	int type = SOCK_STREAM; // TCP
	int protocol = IPPROTO_TCP;

	SOCKET listeningSocket = socket(addressFamily, type, protocol);
	if (listeningSocket == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		return 1;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return 1;
	}
	printf("Listening socket\n");

	// Tell winsock the socket is for listening
	listen(listeningSocket, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);
	if (clientSocket == SOCKET_ERROR)
	{
		printf("client accept failed: %d", WSAGetLastError());
		return 1;
	}

	char host[NI_MAXHOST]; // Client's remote name
	char service[NI_MAXHOST]; // Service (i.e. port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		printf("%s connected on port %s", host, service);
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		printf("%s connected on port %s", host, ntohs(client.sin_port));
	}

	// Close listening socket
	closesocket(listeningSocket);

	// While loop: accept and echo message back to client
	char buf[SOCKET_BUFFER_SIZE];
	int flags = 0;

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, SOCKET_BUFFER_SIZE, flags);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf("Error in recv() Quitting");
			break;
		}
		else if(bytesReceived == 0)
		{
			printf("Client disconnected");
			break;
		}

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, flags);
	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	return 0;
}