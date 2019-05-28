#pragma once

#include "Server_EntitySphere.h"

class Server_DealthBall : public Server_EntitySphere
{
public:
	Server_DealthBall();
	~Server_DealthBall();

	void init(b2World* world, sf::Vector2i position, float size, int scale) override;
	void Kick(sf::Vector2f Direction);

private:
	float speed = 1.5f;
};

