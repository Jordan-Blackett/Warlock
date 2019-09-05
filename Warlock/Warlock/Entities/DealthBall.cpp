#include "DealthBall.h"



void DealthBall::Init(b2World * world, sf::Vector2i position, float size, int scale)
{
	EntitySphere::Init(world, position, size, scale);

	m_Sphere.setFillColor(sf::Color::Red);
}

void DealthBall::Kick(sf::Vector2f Direction)
{
	// Impulse TODO: function
	//body_->ApplyLinearImpulse(b2Vec2(Direction.x, Direction.y), body_->GetWorldCenter(), true);

	b2Vec2 desiredDir = b2Vec2(Direction.x, Direction.y);
	b2Vec2 vel = m_Body->GetLinearVelocity();
	b2Vec2 velChange = b2Vec2(desiredDir.x - vel.x, desiredDir.y - vel.y);
	b2Vec2 impulse = b2Vec2(m_Body->GetMass() * velChange.x, m_Body->GetMass() * velChange.y);
	m_Body->ApplyLinearImpulse(impulse, m_Body->GetWorldCenter(), true);
}
