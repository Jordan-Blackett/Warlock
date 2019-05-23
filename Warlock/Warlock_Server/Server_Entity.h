#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

class Server_Entity
{
public:
	Server_Entity();
	~Server_Entity();

	sf::Vector2i GetPosition();

protected:
	void InitDynamicBody(b2World* world);

	void addRectangularFixtureToBody(sf::Vector2f size);
	void addCircularFixtureToBody(float radius);
	void createFixture(b2Shape * shape);

	//virtual void Update();
	virtual void Render(sf::RenderWindow &Window);

protected:
	// Physics
	b2Body* body_;
	float scale_;

	// 
	sf::Sprite sprite_; // TODO: REMOVE
	sf::Vector2i position_;
};

