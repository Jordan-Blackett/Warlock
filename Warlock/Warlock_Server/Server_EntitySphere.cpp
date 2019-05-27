#include "Server_EntitySphere.h"



Server_EntitySphere::Server_EntitySphere()
{
}


Server_EntitySphere::~Server_EntitySphere()
{
}

void Server_EntitySphere::init(b2World * world, sf::Vector2i position, float size, int scale)
{
	scale_ = scale;
	position_ = position;
	radius_ = size;

	// Rectangle
	InitSphere();

	// Physics
	if (world != nullptr) {
		InitDynamicBody(world);
		addCircularFixtureToBody(radius_);
	}
	else {
		// World = nullptr
	}
}

void Server_EntitySphere::Render(sf::RenderWindow & Window)
{
	sphere_.setPosition(body_->GetPosition().x * scale_, body_->GetPosition().y * scale_);

	Window.draw(sphere_);
}

void Server_EntitySphere::InitSphere()
{
	sphere_.setRadius(radius_);
	sphere_.setOrigin(radius_, radius_);
	sphere_.setPosition(position_.x, position_.y);
	sphere_.setFillColor(sf::Color::Red);
	sphere_.setOutlineThickness(1);
	sphere_.setOutlineColor(sf::Color::White);
}
