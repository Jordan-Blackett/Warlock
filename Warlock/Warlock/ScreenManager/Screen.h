#pragma once

#include <SFML\Graphics.hpp>

class Message;

class Screen
{
public:
	Screen() {};
	~Screen() {};

	virtual void Initialize(sf::RenderWindow* Window) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void OnNotify(Message& message) = 0;

protected:
	sf::RenderWindow* m_Window;
};

