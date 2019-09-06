#pragma once

#include "Screen.h"

#include <cstdint>

#include <Box2D\Box2D.h>

#include "../Entities/Player.h"
#include "../WarlockGame/ArenaSphere.h"
#include "../Entities/DealthBall.h"

//#include "NetworkPlayer.h"
//#include "EntityBox.h"
//
//#include "ScreenManager.h"
//
//#include "Client_InputPacket.h"
//#include "TestPacket.h"

#define ScreenWidth 1024
#define ScreenHeight 768

#define SERVER true

class GameScreen : public Screen
{
public:
	GameScreen() {};
	~GameScreen() {};

	virtual void Initialize(sf::RenderWindow* Window) override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnNotify(Message& message) override;

	void InitPhysicalWorld();

	// Server
	void CreateNewPlayer(unsigned int ClientID);

private:
	// Network
	//std::queue<SnapshotPacket*> snapshotQueue_;

	// Entities
	std::map<uint32_t, Player*> m_ClientEntities;

	// Physics
	b2World* m_World;
	const float m_Scale = 32.f; // Convert between pixel and real-world coordinates
	const float m_VerticalGravity = 0.0f;
	const float m_HorizontalGravity = 0.0f;

	// Physics Tick Rate - Timestep
	sf::Clock m_SimulationClock;
	float m_DT = 1.0f / 60.0f; // Tick rate
	float m_Accumulator;
	double m_SimulationTimeLastUpdated;

	// Level - Arena
	sf::Vector2f m_ScreenCenter;
	float m_RadiusSize;
	ArenaSphere m_ArenaRing;

	//Entities
	// Player
	Player* m_Player = new Player();
	sf::Vector2f m_PlayerSize{ sf::Vector2f(40, 40) };

	DealthBall m_ArenaDeathBall;

	//// Network Players
	////std::vector<NetworkPlayer*> networkPlayers;
	////sf::Vector2f networkPlayersSize{ sf::Vector2f(40, 40) };

	//void onNotify(Message& message)
	//{
	//	//if (message.getMessage() == "Snapshot::")
	//	//{
	//	//	snapshotQueue_.push(message.GetSnapshotPacket());
	//	//}
	//}
};

