#include "GameScreen.h"



GameScreen::GameScreen()
{
}


GameScreen::~GameScreen()
{
}

void GameScreen::Initialize(sf::RenderWindow * Window)
{
	window_ = Window;

	// Prepare the world
	InitPhysicalWorld();

	// Init Player and Boxes
	player->init(world, sf::Vector2i(200, 400), playerSize, scale);

	// Arena
	screenCenter_ = sf::Vector2f(ScreenWidth / 2, ScreenHeight / 2);
	radiusSize_ = screenCenter_.y - 10;
	arenaRing_.init(world, sf::Vector2i(screenCenter_), radiusSize_);

	// Ball
	//arenaDeathBall_.init(world, sf::Vector2i(screenCenter_), 20, scale);
}

void GameScreen::Update()
{
	// Timestep - DeltaTime
	double newTime = simulationClock_.getElapsedTime().asSeconds();
	double frameTime = newTime - simulationTimeLastUpdated_;
	simulationTimeLastUpdated_ = newTime;

	if (frameTime > 0.25f)
		frameTime = 0.25f; // Avoid "Spiral of death"

	accumulator += frameTime;

	while (accumulator >= dt)
	{
		InputPacket2 input = player->movePlayer();

		PacketData data;
		data.input = input;
		TestPacket* testPacket = new TestPacket(data, 10, 0);

		// Send Input to Server
		//std::string msg = ScreenManager::GetInstance()->CreatePacket(0, 10, 0, "");
		//ScreenManager::GetInstance()->SendMessage(msg);
		ScreenManager::GetInstance()->SendMessage(std::string(testPacket->buffer_, testPacket->bufferSize_));
		ScreenManager::GetInstance()->MNotify();

		// Run Simulation
		//world->Step(1 / 60.0f, 8, 3);

		accumulator -= dt;
	}

	// TODO: Function - interp - etcww
	// Consume snapshot
	if (!snapshotQueue_.empty())
	{
		SnapshotPacket* snapshot = snapshotQueue_.front();
		snapshotQueue_.pop();
		if (!snapshot->playerStates.empty())
		{
			for (auto const& p : snapshot->playerStates) {
				if (players.find(p->playerID) != players.end())
				{
					players[p->playerID]->SetPosition(sf::Vector2i(p->positionX, p->positionY));
				}
				else
				{
					Player* newClientPlayer = new Player();
					newClientPlayer->init(world, sf::Vector2i(0, 0), playerSize, scale);
					newClientPlayer->SetPosition(sf::Vector2i(p->positionX, p->positionY));
					players.insert(std::pair<u_int64, Player*>(p->playerID, newClientPlayer));
				}
			}
		}
	}
}

void GameScreen::Render()
{
	arenaRing_.Render(*window_);

	//arenaDeathBall_.Render(*window_);

	for (auto const& p : players) {
		p.second->Render(*window_);
	}
	
	player->Render(*window_);
}

void GameScreen::InitPhysicalWorld()
{
	world = new b2World(b2Vec2(horizontalGravity, verticalGravity));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
}
