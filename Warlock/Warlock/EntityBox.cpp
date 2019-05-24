#include "EntityBox.h"



EntityBox::EntityBox()
{
}


EntityBox::~EntityBox()
{
}

void EntityBox::init(b2World * world, sf::Vector2i position, sf::Vector2f size, int scale)
{
	scale_ = scale;
	position_ = position;
	rectSize_ = size;

	// Rectangle
	InitRectangle();

	// Physics
	if (world != nullptr) {
		InitDynamicBody(world);
		addRectangularFixtureToBody(rectSize_);
	} else {
		// World = nullptr
	}
}

void EntityBox::InitRectangle()
{
	rectangle_.setSize(rectSize_);
	rectangle_.setOrigin(rectSize_.x / 2, rectSize_.y / 2);
	rectangle_.setPosition(position_.x, position_.y);
	rectangle_.setFillColor(sf::Color::Cyan);
}

void EntityBox::Render(sf::RenderWindow &Window)
{
	//rectangle_.setPosition(body_->GetPosition().x * scale_, body_->GetPosition().y * scale_);
	rectangle_.setPosition(position_.x, position_.y);
	//rectangle_.setRotation(body_->GetAngle() * 180 / b2_pi);

	Window.draw(rectangle_);
}
