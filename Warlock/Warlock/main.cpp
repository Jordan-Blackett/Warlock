#include <iostream>
#include <string>
#include <SFML\Graphics.hpp>

#include "ScreenManager/ScreenManager.h"

#pragma comment (lib, "ws2_32.lib")

#include "Network/Network.h"
#include "MessagingSystem/MessagingSystem.h"

#define ScreenWidth 1024
#define ScreenHeight 768

#define SERVER true

int main() {
	std::cout << "Hello World" << std::endl;

	MessagingSystem messageBus;

	// Initialze Network
	ServerNetwork network(&messageBus);
	if (network.Initialize()) {}

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock!");

	// Initialze ScreenManager
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

	system("pause");
	return 0;
}