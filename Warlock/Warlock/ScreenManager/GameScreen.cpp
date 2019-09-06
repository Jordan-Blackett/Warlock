#include "GameScreen.h"

#include "../MessagingSystem/MessagingSystem.h" // TODO: TEMP

void GameScreen::Initialize(sf::RenderWindow * Window)
{
	m_Window = Window;

	// Prepare the world
	InitPhysicalWorld();

	// Init Player and Boxes
	m_Player->Init(m_World, sf::Vector2i(200, 400), m_PlayerSize, m_Scale);

	// Arena
	m_ScreenCenter = sf::Vector2f(ScreenWidth / 2, ScreenHeight / 2);
	m_RadiusSize = m_ScreenCenter.y - 10;
	m_ArenaRing.Init(m_World, sf::Vector2i(m_ScreenCenter), m_RadiusSize);

	// Ball
	m_ArenaDeathBall.Init(m_World, sf::Vector2i(m_ScreenCenter), 20, m_Scale);
}

void GameScreen::Update()
{
	// Timestep - DeltaTime
	double newTime = m_SimulationClock.getElapsedTime().asSeconds();
	double frameTime = newTime - m_SimulationTimeLastUpdated;
	m_SimulationTimeLastUpdated = newTime;

	if (frameTime > 0.25f)
		frameTime = 0.25f; // Avoid "Spiral of death"

	m_Accumulator += frameTime;

	while (m_Accumulator >= m_DT)
	{
		if (SERVER)
		{
			//Input buffer - TODO: Function???
			for (auto const& clientEntity : m_ClientEntities)
			{
				//if (!inputBuffer.Empty(clientEntity.first))
				//{
				//	InputPacket* inputpacket = inputBuffer.GetInputPacket(clientEntity.first);
				//	// Movement
				//	clientEntity.second->MoveEntity(inputpacket, accumulator);

				//	// Abilities
				//	clientEntity.second->Abilities(inputpacket);
				//}
			}
		}
		else
		{
			//InputPacket2 input = player->movePlayer();

			//if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			//{
			//	input.rightclick = true;
			//}

			//PacketData data;
			//data.input = input;
			//TestPacket* testPacket = new TestPacket(data, 10, 0);

			//// Send Input to Server
			////std::string msg = ScreenManager::GetInstance()->CreatePacket(0, 10, 0, "");
			////ScreenManager::GetInstance()->SendMessage(msg);
			//ScreenManager::GetInstance()->SendMessage(std::string(testPacket->buffer_, testPacket->bufferSize_));
			//ScreenManager::GetInstance()->MNotify();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			m_ArenaDeathBall.Kick(sf::Vector2f(0, -7));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			m_ArenaDeathBall.Kick(sf::Vector2f(0, 7));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			m_ArenaDeathBall.Kick(sf::Vector2f(-7, 0));
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_ArenaDeathBall.Kick(sf::Vector2f(7, 0));
		}

		//Run Simulation
		m_World->Step(m_DT, 8, 3);

		// Player update

		if (SERVER)
		{
			// TODO: Send snapshot (FUNCTION)
			//SnapshotPacket* snapshotPacket = new SnapshotPacket();

			// Client Entities
			//for (auto const& obj : clientEntities)
			//{
			//	PlayerState* playerState = new PlayerState();
			//	playerState->playerID = obj.first;
			//	playerState->positionX = obj.second->GetPosition().x;
			//	playerState->positionY = obj.second->GetPosition().y;
			//	playerState->angle = obj.second->GetAngle();
			//	playerState->health = obj.second->GetHealth();

			//	snapshotPacket->playerStates.push_back(playerState);
			//}

			// Objects

			// Send Message To Network
		/*	if (!clientEntities.empty())
			{
				std::string newMessage("Snapshot::");
				SendMessageSystem(Message(newMessage, snapshotPacket));
				Notify();
			}*/
		}

		m_Accumulator -= m_DT;
	}

	if (!SERVER)
	{
		//// TODO: Function - interp - etcww
		//// Consume snapshot
		//if (!snapshotQueue_.empty())
		//{
		//	SnapshotPacket* snapshot = snapshotQueue_.front();
		//	snapshotQueue_.pop();
		//	if (!snapshot->playerStates.empty())
		//	{
		//		for (auto const& p : snapshot->playerStates) {
		//			if (players.find(p->playerID) != players.end())
		//			{
		//				players[p->playerID]->SetPosition(sf::Vector2i(p->positionX, p->positionY));
		//			}
		//			else
		//			{
		//				Player* newClientPlayer = new Player();
		//				newClientPlayer->init(world, sf::Vector2i(0, 0), playerSize, scale);
		//				newClientPlayer->SetPosition(sf::Vector2i(p->positionX, p->positionY));
		//				players.insert(std::pair<u_int64, Player*>(p->playerID, newClientPlayer));
		//			}
		//		}
		//	}
		//}
	}
}

void GameScreen::Render()
{
	m_ArenaRing.Render(*m_Window);

	m_ArenaDeathBall.Render(*m_Window);

	//for (auto const& p : players) {
	//	p.second->Render(*window_);
	//}
	//

	m_Player->Render(*m_Window);

	//for (auto it = clientEntities.begin(); it != clientEntities.end(); ++it)
	//{
	//	it->second->Render(*window_);
	//}
}

void GameScreen::OnNotify(Message& message)
{
	message.GetCategoryFlags();
	message.GetMessageType();

	switch (message.GetMessageType())
	{
		case MessageType::NewConnect:
			CreateNewPlayer(dynamic_cast<NewConnectionMessage*>(&message)->GetSocketIDPacket());
	}

	printf("HEllo");
}

void GameScreen::InitPhysicalWorld()
{
	m_World = new b2World(b2Vec2(m_HorizontalGravity, m_VerticalGravity));
	m_World->SetAllowSleeping(true);
	m_World->SetContinuousPhysics(true);
}

void GameScreen::CreateNewPlayer(unsigned int ClientID)
{
	Player* clientPlayer = new Player();
	clientPlayer->Init(m_World, sf::Vector2i(m_ScreenCenter), m_PlayerSize, m_Scale);
	m_ClientEntities.insert(std::pair<unsigned int, Player*>(ClientID, clientPlayer));
}
