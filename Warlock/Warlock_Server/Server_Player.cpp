#include "Server_Player.h"



Server_Player::Server_Player()
{
}


Server_Player::~Server_Player()
{
}

void Server_Player::init(b2World * world, sf::Vector2i position, sf::Vector2f size, int scale)
{
	Server_EntityBox::init(world, position, size, scale);

	rectangle_.setFillColor(sf::Color::Red);
}

void Server_Player::MoveEntity(InputPacket* input)
{
	enum _moveState {
		MS_STOP,
		MS_UP,
		MS_DOWN,
		MS_LEFT,
		MS_RIGHT,
	};

	_moveState moveState;
	moveState = MS_STOP;

	if (input->up)
	{
		moveState = MS_UP;
	}
	else if (input->down)
	{
		moveState = MS_DOWN;
	}
	else if (input->left)
	{
		moveState = MS_LEFT;
	}
	else if (input->right)
	{
		moveState = MS_RIGHT;
	}

	b2Vec2 vel = body_->GetLinearVelocity();
	b2Vec2 desiredVel = b2Vec2(0, 0);

	switch (moveState)
	{
	case MS_UP: desiredVel.y = -speed; break;
	case MS_DOWN: desiredVel.y = speed; break;
	case MS_LEFT: desiredVel.x = -speed; break;
	case MS_RIGHT: desiredVel.x = speed; break;
	case MS_STOP: desiredVel = b2Vec2(0, 0); break;
	}

	b2Vec2 velChange = b2Vec2(desiredVel.x - vel.x, desiredVel.y - vel.y);

	b2Vec2 impulse = b2Vec2(body_->GetMass() * velChange.x, body_->GetMass() * velChange.y); //disregard time factor
	body_->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), body_->GetWorldCenter(), true);
}
