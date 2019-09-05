#include "ArenaSphere.h"



void ArenaSphere::Init(b2World* world, sf::Vector2i position, float radius)
{
	m_Scale = 32;

	// Create chainshape
	b2ChainShape chainShape;

	// Generate sphere vertices
	m_ArenaSphereVertices = CreateCircle(m_ArenaSphereSides, radius, position);
	chainShape.CreateLoop(m_ArenaSphereVertices.data(), m_ArenaSphereSides);

	// Edge chain
	b2BodyDef BodyDef;
	m_Body = world->CreateBody(&BodyDef);
	m_Body->CreateFixture(&chainShape, 1);

	// Create convex
	m_Convex.setPointCount(m_ArenaSphereSides);
	CreateConvex(m_ArenaSphereVertices.data(), m_ArenaSphereSides);
	m_Convex.setFillColor(sf::Color::Magenta);
}

void ArenaSphere::Render(sf::RenderWindow& window)
{
	window.draw(m_Convex);
}

std::vector<b2Vec2> ArenaSphere::CreateCircle(int sides, float radius, sf::Vector2i offsetPosition)
{
	std::vector<b2Vec2> arenaSphere;
	// Create the circle in the coordinates origin
	for (float a = 0; a < 360; a += 360 / sides)
	{
		float heading = a * 3.1415926535897932384626433832795 / 180;
		arenaSphere.push_back(b2Vec2((offsetPosition.x + (cos(heading) * radius)) / m_Scale, (offsetPosition.y + (sin(heading) * radius)) / m_Scale));
	}

	return arenaSphere;
}

void ArenaSphere::CreateConvex(b2Vec2 * vertices, int points)
{
	for (int i = 0; i < points; i++)
	{
		m_Convex.setPoint(i, sf::Vector2f(vertices[i].x * m_Scale, vertices[i].y * m_Scale));
	}
}
