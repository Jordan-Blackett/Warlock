#include "Player.h"



void Player::Init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale)
{
	EntityBox::Init(world, position, size, scale);

	m_Rectangle.setFillColor(sf::Color::Red);

	//sf::Font font;
	//if (!font.loadFromFile("arial.ttf"))
	//{
	//	// error...
	//}

	//healthTxt.setFont(font);
	//healthTxt.setString("100");
	//healthTxt.setCharacterSize(24);
	//healthTxt.setFillColor(sf::Color::Red);
}

void Player::Render(sf::RenderWindow & window)
{
	EntityBox::Render(window);

	//healthTxt.setString("100");
	//healthTxt.setPosition(10,10);
	//Window.draw(healthTxt);
}

//InputPacket Player::movePlayer()
//{
//	enum _moveState {
//		MS_STOP,
//		MS_UP,
//		MS_DOWN,
//		MS_LEFT,
//		MS_RIGHT,
//	};
//
//	_moveState moveState;
//	moveState = MS_STOP;
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
//	{
//		moveState = MS_UP;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//	{
//		moveState = MS_DOWN;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//	{
//		moveState = MS_LEFT;
//	}
//	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//	{
//		moveState = MS_RIGHT;
//	}
//
//	b2Vec2 vel = body_->GetLinearVelocity();
//	b2Vec2 desiredVel = b2Vec2(0, 0);
//
//	InputPacket2 input;
//
//	//switch (moveState)
//	//{
//	//case MS_UP: desiredVel.y = -speed; input.up = true; break;
//	//	case MS_DOWN: desiredVel.y = speed; break;
//	//	case MS_LEFT: desiredVel.x = -speed; break;
//	//	case MS_RIGHT: desiredVel.x = speed; break;
//	//	case MS_STOP: desiredVel = b2Vec2(0, 0); break;
//	//}
//
//	switch (moveState)
//	{
//		case MS_UP: input.up = true; break;
//		case MS_DOWN: input.down = true; break;
//		case MS_LEFT: input.left = true; break;
//		case MS_RIGHT: input.right = true; break;
//		case MS_STOP: desiredVel = b2Vec2(0, 0); break;
//	}
//
//	b2Vec2 velChange = b2Vec2(desiredVel.x - vel.x, desiredVel.y - vel.y);
//
//	b2Vec2 impulse = b2Vec2(body_->GetMass() * velChange.x, body_->GetMass() * velChange.y); //disregard time factor
//	body_->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), body_->GetWorldCenter(), true);
//
//
//	return input;
//}
