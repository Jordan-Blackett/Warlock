#include "Player.h"



Player::Player()
{
}


Player::~Player()
{
}

void Player::init(b2World * world, sf::Vector2i position, sf::Vector2f size, int scale)
{
	EntityBox::init(world, position, size, scale);

	rectangle_.setFillColor(sf::Color::Red);
}
