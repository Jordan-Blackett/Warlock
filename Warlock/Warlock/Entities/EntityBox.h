#pragma once

#include "Entity.h"

class EntityBox : public Entity
{
public:
	virtual void Init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale);

	virtual void Render(sf::RenderWindow& window) override;
protected:
	void InitRectangle();

protected:
	sf::RectangleShape m_Rectangle;
	sf::Vector2f m_RectSize;
};

