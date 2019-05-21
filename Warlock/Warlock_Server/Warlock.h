#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "BusNode.h"

#include <WinSock2.h>

#include "Server_Player.h"

#include "InputPacket.h"

#define ScreenWidth 1024
#define ScreenHeight 768

//// MOVE TO CONSUMER
//struct InputPacket {
//	uint8_t left = false;
//	uint8_t right = false;
//	uint8_t up = false;
//	uint8_t down = false;
//	uint8_t space = false;
//	uint8_t leftclick = false;
//	uint8_t rightclick = false;
//	uint16_t frameID;
//};

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
	const float verticalGravity = 0.0f;
	const float horizontalGravity = 0.0f;

	// Physics tick rate - Timestep
	sf::Clock simulationClock_;
	float dt = 1.0f / 60.0f; // Tick rate
	float accumulator;

	double simulationTimeLastUpdated_;
	sf::Time simulationTicktime_;

	//Entities
	std::map<u_int64, Server_Player*> clientEntities;

	// Boxes
	sf::Vector2f boxSize{ sf::Vector2f(15, 15) };

	sf::RenderWindow* window_;

	void onNotify(Message message)
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
		//std::cout << "packetType: " << packetType << std::endl;
		memcpy(&packetSubType, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
		packetSubType = ntohs(packetSubType);
		bufferOffset += sizeof(uint16_t);

		switch(packetType)
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
				// TO FUNCTION
				InputPacket newInputPacket;

				memcpy(&newInputPacket.left, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
				bufferOffset += sizeof(uint8_t);
				memcpy(&newInputPacket.right, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
				bufferOffset += sizeof(uint8_t);
				memcpy(&newInputPacket.up, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
				bufferOffset += sizeof(uint8_t);
				memcpy(&newInputPacket.down, message.getMessage().c_str() + bufferOffset, sizeof(uint8_t));
				bufferOffset += sizeof(uint8_t);
				//newInputPacket.left = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
				//newInputPacket.right = (uint8_t)(message.getMessage().c_str() + ++bufferOffset);
				//newInputPacket.up = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
				//newInputPacket.down = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
				
				if (clientEntities.find(ClientID) == clientEntities.end())
				{
					std::cout << "Client ID not found: " << ClientID << std::endl;
				}
				else
				{
					clientEntities[ClientID]->MoveEntity(&newInputPacket);
				}
				//	Add to input buffer
			break;
		}
	}
};

