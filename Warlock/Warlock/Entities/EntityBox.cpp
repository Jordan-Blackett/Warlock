#include "EntityBox.h"



void EntityBox::Init(b2World* world, sf::Vector2i position, sf::Vector2f size, int scale)
{
	m_Scale = scale;
	m_Position = position;
	m_RectSize = size;

	// Rectangle
	InitRectangle();

	// Physics
	if (world != nullptr) {
		InitDynamicBody(world);
		AddRectangularFixtureToBody(m_RectSize);
	}
}

void EntityBox::InitRectangle()
{
	m_Rectangle.setSize(m_RectSize);
	m_Rectangle.setOrigin(m_RectSize.x / 2, m_RectSize.y / 2);
	m_Rectangle.setPosition(m_Position.x, m_Position.y);
	m_Rectangle.setFillColor(sf::Color::Cyan);
}

void EntityBox::Render(sf::RenderWindow &window)
{
	m_Rectangle.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);
	m_Rectangle.setRotation(m_Body->GetAngle() * 180 / b2_pi);

	window.draw(m_Rectangle);
}
