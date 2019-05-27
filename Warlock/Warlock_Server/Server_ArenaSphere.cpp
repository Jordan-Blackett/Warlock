#include "Server_ArenaSphere.h"



Server_ArenaSphere::Server_ArenaSphere()
{
}


Server_ArenaSphere::~Server_ArenaSphere()
{
}

void Server_ArenaSphere::init(b2World * world, sf::Vector2i position, float radius)
{
	b2ChainShape chainShape;

	//Sphere vertices TODO: FUCTION (SIZE)
	convex.setPointCount(sides);


	//blah.setPrimitiveType(sf::LinesStrip);
	//blah.resize(sides);

	// Create the circle in the coordinates origin
	int index = 0;
	for (float a = 0; a < 360 && index < 20; a += 360 / sides)
	{
		float heading = a * 3.1415926535897932384626433832795 / 180;
		vertices[index] = b2Vec2((position.x + (cos(heading) * radius)) / 32, (position.y + (sin(heading) * radius)) / 32);
		//blah[index].position = sf::Vector2f(vertices[index].x, vertices[index].y);
		convex.setPoint(index, sf::Vector2f(vertices[index].x * 32, vertices[index].y * 32));
		index++;
	}

	//vertices[0] = b2Vec2(0 / 32, position.y + 100 / 32);
	//vertices[1] = b2Vec2(position.x / 32, (position.y + 100) / 32);
	////vertices[2] = b2Vec2((position.x + 100) / 32, (position.y + 200) / 32);
	////vertices[3] = b2Vec2((position.x + 400) / 32, (position.y + 200) / 32);
	//blah[0].position = sf::Vector2f(0, position.y + 100);
	//blah[1].position = sf::Vector2f(position.x, position.y + 100);
	////blah[2].position = sf::Vector2f(position.x + 100, position.y + 200);
	////blah[3].position = sf::Vector2f(position.x + 400, position.y + 200);
	chainShape.CreateLoop(vertices, sides);

	//chainShape.CreateChain(vertices, sides);

	//b2Vec2 vs[4];
	//vs[0].Set(6.7f, 5.0f);
	//vs[1].Set(6.0f, 5.25f);
	//vs[2].Set(5.0f, 5.0f);
	//vs[3].Set(1.7f, 5.4f);

	//blah[0].position = sf::Vector2f(vs[0].x * 32, vs[0].y * 32);
	//blah[1].position = sf::Vector2f(vs[1].x * 32, vs[1].y * 32);
	//blah[2].position = sf::Vector2f(vs[2].x * 32, vs[2].y * 32);
	//blah[3].position = sf::Vector2f(vs[3].x * 32, vs[3].y * 32);

	// Edge chain
	b2BodyDef BodyDef;
	//BodyDef.type = b2_staticBody;
	body_ = world->CreateBody(&BodyDef);
	body_->CreateFixture(&chainShape, 1);

	convex.setFillColor(sf::Color::Magenta);
}

void Server_ArenaSphere::Render(sf::RenderWindow & Window)
{
	Window.draw(convex);

	//Window.draw(blah);
}
