#include "Server_DeathBallAnchorPoint.h"



Server_DeathBallAnchorPoint::Server_DeathBallAnchorPoint(sf::Vector2i position, float scale) : position_(position), scale_(scale)
{
}


Server_DeathBallAnchorPoint::~Server_DeathBallAnchorPoint()
{
}

void Server_DeathBallAnchorPoint::InitStaticBody(b2World * world)
{
	// Add a static body to world
	b2BodyDef BodyDef;
	BodyDef.type = b2_staticBody;
	BodyDef.position = b2Vec2(position_.x / scale_, position_.y / scale_);
	body_ = world->CreateBody(&BodyDef);
}

b2Body * Server_DeathBallAnchorPoint::GetBody()
{
	return body_;
}
