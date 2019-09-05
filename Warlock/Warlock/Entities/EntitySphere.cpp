#include "EntitySphere.h"



void EntitySphere::Init(b2World * world, sf::Vector2i position, float size, int scale)
{
	m_Scale = scale;
	m_Position = position;
	m_Radius = size;

	// Rectangle
	InitSphere();

	// Physics
	if (world != nullptr) {
		InitDynamicBody(world);
		AddCircularFixtureToBody(m_Radius);
	}
}

void EntitySphere::Render(sf::RenderWindow& window)
{
	m_Sphere.setPosition(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale);

	window.draw(m_Sphere);
}

void EntitySphere::InitSphere()
{
	m_Sphere.setRadius(m_Radius);
	m_Sphere.setOrigin(m_Radius, m_Radius);
	m_Sphere.setPosition(m_Position.x, m_Position.y);
	m_Sphere.setFillColor(sf::Color::Red);
	m_Sphere.setOutlineThickness(1);
	m_Sphere.setOutlineColor(sf::Color::White);
}
