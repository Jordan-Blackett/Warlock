#pragma once

#include "EntitySphere.h"

class DealthBall : public EntitySphere
{
public:
	virtual void Init(b2World* world, sf::Vector2i position, float size, int scale) override;
	void Kick(sf::Vector2f Direction);

private:
	float m_Speed = 1.5f;
};
