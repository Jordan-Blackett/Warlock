#include "Warlock.h"



Warlock::Warlock(MessagingSystem* messageBus) : BusNode(messageBus)
{
}


Warlock::~Warlock()
{
}

bool Warlock::Init()
{
	window_ = new sf::RenderWindow(sf::VideoMode(ScreenWidth, ScreenHeight, 32), "Warlock Server!");

	// Prepare the world
	InitPhysicalWorld();

	// Prepare the level
	screenCenter_ = sf::Vector2f(ScreenWidth / 2, ScreenHeight / 2);
	// Function

	// Arena
	//radiusSize_ = screenCenter_.y - 10;
	//arenaRing_.setRadius(radiusSize_);
	//arenaRing_.setPosition(screenCenter_);
	//arenaRing_.setOrigin(radiusSize_, radiusSize_);
	//arenaRing_.setFillColor(sf::Color::Blue);
	//arenaRing_.setOutlineThickness(10);
	//arenaRing_.setOutlineColor(sf::Color::White);

	// Collision

	test.init(world, sf::Vector2i(screenCenter_), screenCenter_.y - 10);

	// Ball
	//arenaDeathBallAnchorPoint_ = new Server_DeathBallAnchorPoint(sf::Vector2i(screenCenter_), scale);
	//arenaDeathBallAnchorPoint_->InitStaticBody(world);

	arenaDeathBall_.init(world, sf::Vector2i(screenCenter_), 20, scale, nullptr);

	return true;
}

void Warlock::InitPhysicalWorld()
{
	world = new b2World(b2Vec2(horizontalGravity, verticalGravity));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
}

void Warlock::Run()
{
	while (window_->isOpen())
	{
		sf::Event event;
		while (window_->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window_->close();
		}

		// ---Game Loop---
		// Timestep - DeltaTime
		double newTime = simulationClock_.getElapsedTime().asSeconds();
		double frameTime = newTime - simulationTimeLastUpdated_;
		simulationTimeLastUpdated_ = newTime;

		if (frameTime > 0.25f)
			frameTime = 0.25f; // Avoid "Spiral of death"

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			// Init new players - while in lobby mode

			//Input buffer
			//player->movePlayer();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				arenaDeathBall_.Kick(sf::Vector2f(0, -7));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				arenaDeathBall_.Kick(sf::Vector2f(0, 7));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				arenaDeathBall_.Kick(sf::Vector2f(-7, 0));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				arenaDeathBall_.Kick(sf::Vector2f(7, 0));
			}

			//arenaDeathBall_.Kick(sf::Vector2f(1, 0));
			//deathwall.Kick(sf::Vector2f(-1, 0));

			//Run Simulation
			world->Step(1 / 60.0f, 8, 3);

			// Player update


			// TODO: Send snapshot (FUNCTION)
			SnapshotPacket* snapshotPacket = new SnapshotPacket();

			// Client Entities
			for (auto const& obj : clientEntities)
			{
				ObjectState* objectState = new ObjectState();
				objectState->positionX = obj.second->GetPosition().x;
				objectState->positionY = obj.second->GetPosition().y;

				snapshotPacket->objectStates.push_back(objectState);
			}

			// Objects


			// Send Message To Network
			std::string newMessage("Snapshot::");
			SendMessageSystem(Message(newMessage, snapshotPacket));
			Notify();

			accumulator -= dt;
		}

		// Render
		window_->clear();

		test.Render(*window_);

		arenaDeathBall_.Render(*window_);

		for (auto it = clientEntities.begin(); it != clientEntities.end(); ++it)
		{
			it->second->Render(*window_);
		}
		window_->display();
	}
}

void Warlock::CreateNewPlayer(unsigned int ClientID)
{
	Server_Player* box = new Server_Player();
	box->init(world, sf::Vector2i(screenCenter_), boxSize, scale);
	clientEntities.insert(std::pair<unsigned int, Server_Player*>(ClientID, box));
}

void Warlock::BufferToInputPacket()
{
	InputPacket newInputPacket;

	//newInputPacket.left = 
	//memcpy(buffer_ + bufferOffset_, &data.input.left, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.right, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.up, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.down, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.space, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.leftclick, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
	//memcpy(buffer_ + bufferOffset_, &data.input.rightclick, sizeof(uint8_t));
	//bufferOffset_ += sizeof(uint8_t);
}

void Warlock::onNotify(Message message)
{
	int bufferOffset = 0;
	uint16_t ClientID = 0;
	uint16_t packetType = 0;
	uint16_t packetSubType = 0;

	// Client ID
	memcpy(&ClientID, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	ClientID = ntohs(ClientID); // TODO: not necessary as there's only one hton call 
	bufferOffset += sizeof(uint16_t);

	// Type/ Sub-Type
	memcpy(&packetType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	packetType = ntohs(packetType);
	bufferOffset += sizeof(uint16_t);
	memcpy(&packetSubType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
	packetSubType = ntohs(packetSubType);
	bufferOffset += sizeof(uint16_t);

	switch (packetType)
	{
		// New_Connection
	case 0:
		// Player already exist
		if (clientEntities.find(ClientID) == clientEntities.end())
		{
			std::cout << "New Client ID: " << ClientID << std::endl;
			CreateNewPlayer(ClientID);
		}
		break;
		// Input Packet
	case 10:
		// TODO: TO FUNCTION
		InputPacket newInputPacket;

		memcpy(&newInputPacket.left, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
		bufferOffset += sizeof(uint8_t);
		memcpy(&newInputPacket.right, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
		bufferOffset += sizeof(uint8_t);
		memcpy(&newInputPacket.up, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
		bufferOffset += sizeof(uint8_t);
		memcpy(&newInputPacket.down, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
		bufferOffset += sizeof(uint8_t);


		// TODO: Add to input buffer
		if (clientEntities.find(ClientID) == clientEntities.end())
		{
			std::cout << "Client ID not found: " << ClientID << std::endl;
		}
		else
		{
			clientEntities[ClientID]->MoveEntity(&newInputPacket);
		}

		break;
	}
}
