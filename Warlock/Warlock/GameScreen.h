#pragma once

#include "Screen.h"

#include <Box2D\Box2D.h>

#include "Player.h"
//#include "NetworkPlayer.h"
#include "EntityBox.h"

#include "ScreenManager.h"

#include "Client_InputPacket.h"
#include "TestPacket.h"

#include "Client_ArenaSphere.h"

class GameScreen : public Screen
{
public:
	GameScreen();
	~GameScreen();

	void Initialize(sf::RenderWindow* Window) override;
	void Update() override;
	void Render() override;

	void InitPhysicalWorld();

private:
	// Network
	std::queue<SnapshotPacket*> snapshotQueue_;

	// Physics
	b2World * world;
	const float scale = 32.f; // Convert between pixel and real-world coordinates
	const float verticalGravity = 0.0f;
	const float horizontalGravity = 0.0f;

	// Physics tick rate - Timestep
	sf::Clock simulationClock_;
	float dt = 1.0f / 60.0f; // Tick rate
	float accumulator;

	double simulationTimeLastUpdated_;
	sf::Time simulationTicktime_;

	// Level
	sf::Vector2f screenCenter_;
	Client_ArenaSphere arenaRing_;
	float radiusSize_;

	//Server_DealthBall arenaDeathBall_;

	//Entities
	// Player
	Player* player = new Player();
	sf::Vector2f playerSize{ sf::Vector2f(40, 40) };

	// Network Players
	//std::vector<NetworkPlayer*> networkPlayers;
	//sf::Vector2f networkPlayersSize{ sf::Vector2f(40, 40) };

	void onNotify(Message message)
	{
		if (message.getMessage() == "Snapshot::")
		{
			snapshotQueue_.push(message.GetSnapshotPacket());
		}
	}
};

