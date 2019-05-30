#pragma once

#include "EntityBox.h"
#include "TestPacket.h"

class Player : public EntityBox
{
public:
	Player();
	~Player();

	void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale) override;
	void Render(sf::RenderWindow &Window) override;

	InputPacket2 movePlayer();

private:
	// Stats
	int health_ = 100;
	float speed = 4.0f;

	// TODO: move to ui class
	sf::Text healthTxt;

};

