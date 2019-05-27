#pragma once

#include "Entity.h"

class EntityBox : public Entity
{
public:
	EntityBox();
	~EntityBox();

	virtual void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale);

	void Render(sf::RenderWindow &Window) override;
protected:
	void InitRectangle();

protected:
	sf::Vector2f rectSize_;
	sf::RectangleShape rectangle_;
};

