#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "ScreenManager.h"

#pragma comment (lib, "ws2_32.lib")
#include "Network.h"
#include "Client_MessagingSystem.h"

#define ScreenWidth 1024
#define ScreenHeight 768

int main() {
	std::cout << "Hello World Client" << std::endl;

	Client_MessagingSystem messageBus;

	// Initialze Network
	Network clientNetwork(&messageBus);
	if (clientNetwork.Init()) {}

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock!");

	// ScreenManager
	ScreenManager::GetInstance()->Initialize(&messageBus, &window);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Update
		ScreenManager::GetInstance()->Update();

		// Render
		window.clear();
		ScreenManager::GetInstance()->Render();
		window.display();
	}


	//printf("Connected to the server\n");

	// Do-While loop to send and receive data
	//char buf[SOCKET_BUFFER_SIZE];
	//std::string userInput;

	//while (true)
	//{
	//	// Prompt the user for some text
	//	std::cout << "> ";
	//	std::getline(std::cin, userInput);

	//	if (userInput.size() > 0)
	//	{
	//		//std::string msg("::position::testtemp");

			//PacketData data;
			//InputPacket2 input;
			//input.up = true;
			//data.input = input;
			//TestPacket* testPacket = new TestPacket(data);

	//		//send(clientSocket, testPacket->buffer_, testPacket->bufferSize_, 0);

	//		// Write out to that socket
	//		//string s(argv[1]);
	//		//int sendOk = sendto(UDPclientSocket, testPacket->buffer_, testPacket->bufferSize_, 0, (sockaddr*)&server, sizeof(server));

	//		// Send the text
	//		//if (send(clientSocket, userInput.c_str(), userInput.size() + 1, 0) == SOCKET_ERROR);
	//		//{
	//		//	// Wait for response
	//		//	ZeroMemory(buf, SOCKET_BUFFER_SIZE);
	//		//	int bytesReceived = recv(clientSocket, buf, 4096, 0);

	//		//	if (bytesReceived > 0)
	//		//	{
	//		//		// Echo response to console
	//		//		std::cout << std::string(buf, 0, bytesReceived) << std::endl;
	//		//	}
	//		//}
	//	}
	//}

	//// Cleanup winsock
	////closesocket(clientSocket);
	//WSACleanup();
	//printf("Cleanup\n");

	return 0;
}