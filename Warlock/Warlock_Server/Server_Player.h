#pragma once

#include "Server_EntityBox.h"
#include "InputPacket.h"

class Server_Player : public Server_EntityBox
{
public:
	Server_Player();
	~Server_Player();

	void init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale) override;
	void MoveEntity(InputPacket* input, double deltaTime);

private:
	float speed = 10.5f;


	float maxSpeed = 4.5f;
};

