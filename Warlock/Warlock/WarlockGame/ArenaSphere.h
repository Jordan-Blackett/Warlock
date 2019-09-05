#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class ArenaSphere
{
public:
	ArenaSphere() {};
	~ArenaSphere() {};

	void Init(b2World* world, sf::Vector2i position, float radius);

	void Render(sf::RenderWindow& window);

	std::vector<b2Vec2> CreateCircle(int sides, float radius, sf::Vector2i offsetPosition);
	void CreateConvex(b2Vec2* vertices, int points);

private:
	b2Body* m_Body;
	std::vector<b2Vec2> m_ArenaSphereVertices;
	int m_ArenaSphereSides = 20;
	int m_Scale;

	sf::ConvexShape m_Convex;
};

