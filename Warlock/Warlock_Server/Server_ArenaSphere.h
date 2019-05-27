#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class Server_ArenaSphere
{
public:
	Server_ArenaSphere();
	~Server_ArenaSphere();

	void init(b2World * world, sf::Vector2i position, float radius);

	void Render(sf::RenderWindow &Window);

private:
	b2Body* body_;


	int sides = 20;

	sf::ConvexShape convex;
	b2Vec2 vertices[20];
	sf::VertexArray blah;
};

