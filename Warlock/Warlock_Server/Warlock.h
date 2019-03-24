#pragma once

#include <SFML\Graphics.hpp>
//#include <Box2D\Box2D.h>

#define ScreenWidth 1024
#define ScreenHeight 768

class Warlock
{
public:
	Warlock();
	~Warlock();

	bool Init();

	void Run();

private:
	sf::RenderWindow* window_;

};

