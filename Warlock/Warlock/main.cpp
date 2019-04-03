#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>

#include "ScreenManager.h"


#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

#include "TestPacket.h"

#pragma comment (lib, "ws2_32.lib")

#include <Box2D\Box2D.h>

#include "EntityBox.h"

#define ScreenWidth 1024
#define ScreenHeight 768

#define PORT 54000
#define SOCKET_BUFFER_SIZE 4096

int main() {
	std::cout << "Hello World" << std::endl;

	std::string ipAddress = "127.0.0.1";
	int port = PORT;

	// Initialze winsock
	// Initialze winsock
	WSADATA winsockData;
	WORD winsockVersion = MAKEWORD(2, 2); //0x202;

	if (WSAStartup(winsockVersion, &winsockData))
	{
		printf("WSAStartup failed: %d", WSAGetLastError());
		return 1;
	}

	//// Create a socket - TCP
	//int addressFamily = AF_INET;
	//int type = SOCK_STREAM; // TCP
	//int protocol = IPPROTO_TCP;

	//SOCKET clientSocket = socket(addressFamily, type, protocol);
	//if (clientSocket == INVALID_SOCKET)
	//{
	//	printf("socket failed: %d", WSAGetLastError());
	//	WSACleanup();
	//	return 1;
	//}

	//// Fill in a hint structure
	//sockaddr_in hint;
	//hint.sin_family = AF_INET;
	//hint.sin_port = htons(PORT);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	//if (connect(clientSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	//{
	//	printf("Failed to connect to the server: %d", WSAGetLastError());
	//	closesocket(clientSocket);
	//	WSACleanup();
	//	return 1;
	//}

	// Create a hint structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(52000); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array

	SOCKET UDPclientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (UDPclientSocket == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		WSACleanup();
		return 1;
	}


	printf("Connected to the server\n");

	// Do-While loop to send and receive data
	char buf[SOCKET_BUFFER_SIZE];
	std::string userInput;

	while (true)
	{
		// Prompt the user for some text
		std::cout << "> ";
		std::getline(std::cin, userInput);

		if (userInput.size() > 0)
		{
			std::string msg("::position::testtemp");

			PacketData data;
			InputPacket2 input;
			input.up = true;
			data.input = input;
			TestPacket* testPacket = new TestPacket(data);

			//send(clientSocket, testPacket->buffer_, testPacket->bufferSize_, 0);

			// Write out to that socket
			//string s(argv[1]);
			int sendOk = sendto(UDPclientSocket, testPacket->buffer_, testPacket->bufferSize_, 0, (sockaddr*)&server, sizeof(server));


			// Send the text
			//if (send(clientSocket, userInput.c_str(), userInput.size() + 1, 0) == SOCKET_ERROR);
			//{
			//	// Wait for response
			//	ZeroMemory(buf, SOCKET_BUFFER_SIZE);
			//	int bytesReceived = recv(clientSocket, buf, 4096, 0);

			//	if (bytesReceived > 0)
			//	{
			//		// Echo response to console
			//		std::cout << std::string(buf, 0, bytesReceived) << std::endl;
			//	}
			//}
		}
	}

	// Cleanup winsock
	//closesocket(clientSocket);
	WSACleanup();
	printf("Cleanup\n");

	//sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock!");

	//// ScreenManager
	//ScreenManager::GetInstance()->Initialize(&window);

	//while (window.isOpen())
	//{
	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();
	//	}

	//	// Update
	//	ScreenManager::GetInstance()->Update();

	//	// Render
	//	window.clear();
	//	ScreenManager::GetInstance()->Render();
	//	window.display();
	//}

	while (true)
	{
	}

	return 0;
}