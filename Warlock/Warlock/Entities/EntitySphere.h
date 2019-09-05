#pragma once

#include "Entity.h"

class EntitySphere : public Entity
{
public:
	virtual void Init(b2World* world, sf::Vector2i position, float size, int scale);

	virtual void Render(sf::RenderWindow& window) override;

protected:
	void InitSphere();

protected:
	sf::CircleShape m_Sphere;
	float m_Radius;
};

