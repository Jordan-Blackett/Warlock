#pragma once

#include "Server_Entity.h"

class Server_EntitySphere : public Server_Entity
{
public:
	Server_EntitySphere();
	~Server_EntitySphere();

	virtual void init(b2World* world, sf::Vector2i position, float size, int scale);

	void Render(sf::RenderWindow &Window) override;

protected:
	void InitSphere();

protected:
	float radius_;
	sf::CircleShape sphere_;
};

