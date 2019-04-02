#pragma once

#include "Server_Entity.h"

class Server_EntityBox : public Server_Entity
{
public:
	Server_EntityBox();
	~Server_EntityBox();

	virtual void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale);

	void Render(sf::RenderWindow &Window) override;
protected:
	void InitRectangle();

	sf::Vector2f rectSize_;
	sf::RectangleShape rectangle_;
};

