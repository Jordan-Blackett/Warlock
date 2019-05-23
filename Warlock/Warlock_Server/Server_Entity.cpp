#include "Server_Entity.h"



Server_Entity::Server_Entity()
{
}


Server_Entity::~Server_Entity()
{
}

void Server_Entity::InitDynamicBody(b2World * world)
{
	// Add a dynamic body to world
	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position = b2Vec2(position_.x / scale_, position_.y / scale_);
	body_ = world->CreateBody(&BodyDef);
}

void Server_Entity::addRectangularFixtureToBody(sf::Vector2f size)
{
	b2PolygonShape shape;
	shape.SetAsBox((size.x / 2) / scale_, (size.y / 2) / scale_);
	createFixture(&shape);
}

void Server_Entity::addCircularFixtureToBody(float radius)
{
	b2CircleShape shape;
	shape.m_radius = radius * scale_;
	createFixture(&shape);
}

void Server_Entity::createFixture(b2Shape* shape)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.7f;
	body_->CreateFixture(&fixtureDef);
}

void Server_Entity::Render(sf::RenderWindow & Window)
{
}

sf::Vector2i Server_Entity::GetPosition()
{
	return position_;
}
