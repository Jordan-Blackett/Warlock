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
	//InitPhysicalWorld();

	return true;
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
		//double newTime = simulationClock_.getElapsedTime().asSeconds();
		//double frameTime = newTime - simulationTimeLastUpdated_;
		//simulationTimeLastUpdated_ = newTime;

		//if (frameTime > 0.25f)
		//	frameTime = 0.25f; // Avoid "Spiral of death"

		//accumulator += frameTime;

		//while (accumulator >= dt)
		//{

		// Init new players - while in lobby mode

		// Input buffer
		//	player->movePlayer();

		// Run Simulation
		//world->Step(1 / 60.0f, 8, 3);

		// Send snapshot

		//	accumulator -= dt;
		//}

		//notify();

		// Render
		window_->clear();
		//ScreenManager::GetInstance()->Render();
		window_->display();
	}
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
