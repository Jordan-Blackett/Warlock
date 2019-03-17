#include <iostream>
#include <SFML\Graphics.hpp>

#include "ScreenManager.h"


#include <Box2D\Box2D.h>

#include "EntityBox.h"

#define ScreenWidth 1024
#define ScreenHeight 768

int main() {
	std::cout << "Hello World" << std::endl;

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock!");

	// ScreenManager
	ScreenManager::GetInstance()->Initialize(&window);

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

	return 0;
}