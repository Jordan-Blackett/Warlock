#pragma once

#include <SFML\Graphics.hpp>

#define ScreenWidth 1024
#define ScreenHeight 768

class Screen
{
public:
	Screen();
	~Screen();

	virtual void Initialize(sf::RenderWindow* Window) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	sf::RenderWindow* window_;
};

