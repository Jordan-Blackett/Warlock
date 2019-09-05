#include "Entity.h"



void Entity::InitDynamicBody(b2World* world)
{
	// Add a dynamic body to world
	b2BodyDef BodyDef;
	BodyDef.type = b2_dynamicBody;
	BodyDef.position = b2Vec2(m_Position.x / m_Scale, m_Position.y / m_Scale);
	m_Body = world->CreateBody(&BodyDef);

	// No Gravity - Friction
	m_Body->SetLinearDamping(0.4f);
	m_Body->SetAngularDamping(0.4f);
}

void Entity::AddRectangularFixtureToBody(sf::Vector2f& size)
{
	b2PolygonShape shape;
	shape.SetAsBox((size.x / 2) / m_Scale, (size.y / 2) / m_Scale);
	CreateFixture(&shape);
}

void Entity::AddCircularFixtureToBody(float radius)
{
	b2CircleShape shape;
	shape.m_radius = radius / m_Scale;
	CreateFixture(&shape);
}

void Entity::CreateFixture(b2Shape* shape)
{
	b2FixtureDef fixtureDef;
	fixtureDef.shape = shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction =  0.7f;
	m_Body->CreateFixture(&fixtureDef);
}

void Entity::SetPosition(sf::Vector2i& pos)
{
	// Interpolation

	//m_Position = pos;
}
