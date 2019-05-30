#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D/Box2D.h>

class Client_ArenaSphere
{
public:
	Client_ArenaSphere();
	~Client_ArenaSphere();

	void init(b2World * world, sf::Vector2i position, float radius);

	void Render(sf::RenderWindow &Window);

	std::vector<b2Vec2> CreateCircle(int sides, float radius, sf::Vector2i offsetPosition);
	void CreateConvex(b2Vec2* vertices, int points);

private:
	b2Body * body_;
	std::vector<b2Vec2> arenaSphereVertices_;
	int arenaSphereSides_ = 20;
	int scale_;

	sf::ConvexShape convex_;
};

