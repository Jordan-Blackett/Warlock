#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

void Entity::InitDynamicBody(b2World * world)
{
	// Add a dynamic body to world
	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position = b2Vec2(position_.x / scale_, position_.y / scale_);
	body_ = world->CreateBody(&BodyDef);
}

void Entity::addRectangularFixtureToBody(sf::Vector2f size)
{
	b2PolygonShape shape;
	shape.SetAsBox((size.x / 2) / scale_, (size.y / 2) / scale_);
	createFixture(&shape);
}

void Entity::addCircularFixtureToBody(float radius)
{
	b2CircleShape shape;
	shape.m_radius = radius * scale_;
	createFixture(&shape);
}

void Entity::createFixture(b2Shape* shape)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction =  0.7f;
	body_->CreateFixture(&fixtureDef);
}

void Entity::Render(sf::RenderWindow & Window)
{
}

void Entity::SetPosition(sf::Vector2i pos)
{
	position_ = pos;
}
