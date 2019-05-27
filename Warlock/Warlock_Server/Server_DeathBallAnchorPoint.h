#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class Server_DeathBallAnchorPoint
{
public:
	Server_DeathBallAnchorPoint(sf::Vector2i position, float scale);
	~Server_DeathBallAnchorPoint();

	void InitStaticBody(b2World* world);

	b2Body* GetBody();

private:
	b2Body * body_;
	float scale_;

	sf::Vector2i position_;
};

