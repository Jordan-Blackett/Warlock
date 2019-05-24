#pragma once

#include <SFML\Graphics.hpp>
#include "Client_MessagingSystem.h"

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

	virtual void onNotify(Message message) = 0;

protected:
	sf::RenderWindow* window_;
};

