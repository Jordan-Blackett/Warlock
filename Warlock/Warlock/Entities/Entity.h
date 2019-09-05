#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class Entity
{
public:
	Entity() {};
	~Entity() {};

	sf::Vector2i GetPosition() { return sf::Vector2i(m_Body->GetPosition().x * m_Scale, m_Body->GetPosition().y * m_Scale); }
	float GetAngle() { m_Body->GetAngle(); }
	void SetPosition(sf::Vector2i& pos);

protected:
	void InitDynamicBody(b2World* world);

	void AddRectangularFixtureToBody(sf::Vector2f& size);
	void AddCircularFixtureToBody(float radius);
	void CreateFixture(b2Shape* shape);

	virtual void Render(sf::RenderWindow& Window) {};

protected:
	// Physics
	b2Body* m_Body;
	float m_Scale;

	// 
	sf::Sprite m_Sprite;
	sf::Vector2i m_Position;
};

