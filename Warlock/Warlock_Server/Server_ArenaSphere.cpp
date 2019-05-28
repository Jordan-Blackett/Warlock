#include "Server_ArenaSphere.h"



Server_ArenaSphere::Server_ArenaSphere()
{
}


Server_ArenaSphere::~Server_ArenaSphere()
{
}

void Server_ArenaSphere::init(b2World * world, sf::Vector2i position, float radius)
{
	scale_ = 32;

	// Create chainshape
	b2ChainShape chainShape;

	// Generate sphere vertices
	arenaSphereVertices_ = CreateCircle(arenaSphereSides_, radius, position);
	chainShape.CreateLoop(arenaSphereVertices_.data(), arenaSphereSides_);

	// Edge chain
	b2BodyDef BodyDef;
	body_ = world->CreateBody(&BodyDef);
	body_->CreateFixture(&chainShape, 1);

	// Create convex
	convex_.setPointCount(arenaSphereSides_);
	CreateConvex(arenaSphereVertices_.data(), arenaSphereSides_);
	convex_.setFillColor(sf::Color::Magenta);
}

void Server_ArenaSphere::Render(sf::RenderWindow & Window)
{
	Window.draw(convex_);
}

std::vector<b2Vec2> Server_ArenaSphere::CreateCircle(int sides, float radius, sf::Vector2i offsetPosition)
{
	std::vector<b2Vec2> arenaSphere;
	// Create the circle in the coordinates origin
	for (float a = 0; a < 360; a += 360 / sides)
	{
		float heading = a * 3.1415926535897932384626433832795 / 180;
		arenaSphere.push_back(b2Vec2((offsetPosition.x + (cos(heading) * radius)) / scale_, (offsetPosition.y + (sin(heading) * radius)) / scale_));
	}

	return arenaSphere;
}

void Server_ArenaSphere::CreateConvex(b2Vec2 * vertices, int points)
{
	for (int i = 0; i < points; i++)
	{
		convex_.setPoint(i, sf::Vector2f(vertices[i].x * scale_, vertices[i].y * scale_));
	}
}
