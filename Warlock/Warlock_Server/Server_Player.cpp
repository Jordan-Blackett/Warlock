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

	//sf::Font font;
	//if (!font.loadFromFile("arial.ttf"))
	//{
	//	// error...
	//}

	//healthTxt.setFont(font);
	healthTxt.setString("100");
	healthTxt.setCharacterSize(24);
	healthTxt.setFillColor(sf::Color::Red);
}

void Server_Player::Render(sf::RenderWindow & Window)
{
	Server_EntityBox::Render(Window);

	healthTxt.setString("100");
	healthTxt.setPosition(10, 10);
	Window.draw(healthTxt);
}

void Server_Player::MoveEntity(InputPacket* input, double deltaTime)
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

	if (moveState != MS_STOP) {
		//b2Vec2 velChange = b2Vec2(desiredVel.x - vel.x, desiredVel.y - vel.y);
		//b2Vec2 impulse = b2Vec2(body_->GetMass() * velChange.x, body_->GetMass() * velChange.y); //disregard time factor
		

		//b2Vec2 acceleration = b2Vec2(desiredVel.x,  desiredVel.y); // acc

		// Current velocity
		const b2Vec2 currentVelocity = body_->GetLinearVelocity();

		// Calculate friction
		b2Vec2 friction = b2Vec2(0.01, 0.01);
		friction.x = -currentVelocity.x * friction.x;
		friction.y = -currentVelocity.y * friction.y;

		// Apply friction
		desiredVel += friction;

		// Calculate acceleration
		const b2Vec2 acceleration = desiredVel;

		// Calculate new velocity based on acceleration
		b2Vec2 newVelocity;
		newVelocity.x = currentVelocity.x + (acceleration.x * deltaTime);
		newVelocity.y = currentVelocity.y + (acceleration.y * deltaTime);
		body_->SetLinearVelocity(newVelocity);

		//body_->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), body_->GetWorldCenter(), true);
	}

	// TODO: replace getpos fun
	position_.x = body_->GetPosition().x * scale_;
	position_.y = body_->GetPosition().y * scale_;
}

int Server_Player::GetHealth()
{
	return health_;
}
