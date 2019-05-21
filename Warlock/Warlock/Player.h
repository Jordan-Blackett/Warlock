#pragma once

#include "EntityBox.h"
#include "TestPacket.h"

class Player : public EntityBox
{
public:
	Player();
	~Player();

	void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale) override;
	InputPacket2 movePlayer();

private:
	float speed = 4.0f;

};

