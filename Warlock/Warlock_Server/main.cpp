#include <iostream>
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#define ScreenWidth 1024
#define ScreenHeight 768

int main() {
	std::cout << "Hello World" << std::endl;

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock_Server!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Cyan);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}