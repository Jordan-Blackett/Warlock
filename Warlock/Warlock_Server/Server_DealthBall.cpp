#include "Server_DealthBall.h"



Server_DealthBall::Server_DealthBall()
{
}


Server_DealthBall::~Server_DealthBall()
{
}

void Server_DealthBall::init(b2World * world, sf::Vector2i position, float size, int scale)
{
	Server_EntitySphere::init(world, position, size, scale);

	sphere_.setFillColor(sf::Color::Red);
}

void Server_DealthBall::Kick(sf::Vector2f Direction)
{
	// Impulse TODO: function
	//body_->ApplyLinearImpulse(b2Vec2(Direction.x, Direction.y), body_->GetWorldCenter(), true);

	b2Vec2 desiredDir = b2Vec2(Direction.x, Direction.y);
	b2Vec2 vel = body_->GetLinearVelocity();
	b2Vec2 velChange = b2Vec2(desiredDir.x - vel.x, desiredDir.y - vel.y);
	b2Vec2 impulse = b2Vec2(body_->GetMass() * velChange.x, body_->GetMass() * velChange.y);
	body_->ApplyLinearImpulse(impulse, body_->GetWorldCenter(), true);
}
