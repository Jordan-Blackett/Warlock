#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "BusNode.h"

#include <WinSock2.h>

#include "Server_Player.h"
#include "Server_Dealthball.h"
#include "Server_ArenaSphere.h"

#include "InputPacket.h"

#include "Server_InputBuffer.h"
#include "Server_SnapshotPacket.h"

#define ScreenWidth 1024
#define ScreenHeight 768

class Warlock : public BusNode
{
public:
	Warlock(MessagingSystem* messageBus);
	~Warlock();

	bool Init();
	void InitPhysicalWorld();

	void Run();

	void CreateNewPlayer(unsigned int ClientID);

	void BufferToInputPacket();

private:
	// Physics
	b2World * world;
	const float scale = 32.f; // Convert between pixel and real-world coordinates
	const float verticalGravity = 1.f;
	const float horizontalGravity = 0.0f;

	// Physics tick rate - Timestep
	sf::Clock simulationClock_;
	float dt = 1.0f / 60.0f; // Tick rate
	float accumulator;

	double simulationTimeLastUpdated_;
	sf::Time simulationTicktime_;

	// Level
	sf::Vector2f screenCenter_;
	Server_ArenaSphere arenaRing_;
	float radiusSize_;

	Server_DealthBall arenaDeathBall_;

	//Entities
	std::map<u_int64, Server_Player*> clientEntities;

	// Boxes
	sf::Vector2f boxSize{ sf::Vector2f(15, 15) };

	sf::RenderWindow* window_;

	void onNotify(Message message);

	Server_InputBuffer inputBuffer;
};

