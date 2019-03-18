#pragma once

#include "EntityBox.h"

class Player : public EntityBox
{
public:
	Player();
	~Player();

	void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale) override;
	void movePlayer();

private:
	float speed = 4.0f;

};

