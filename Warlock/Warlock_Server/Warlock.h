#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "BusNode.h"

#define ScreenWidth 1024
#define ScreenHeight 768

// MOVE TO CONSUMER
struct InputPacket {
	uint8_t left = false;
	uint8_t right = false;
	uint8_t up = false;
	uint8_t down = false;
	uint8_t space = false;
	uint8_t leftclick = false;
	uint8_t rightclick = false;
	uint16_t frameID;
};

class Warlock : public BusNode
{
public:
	Warlock(MessagingSystem* messageBus);
	~Warlock();

	bool Init();

	void Run();

	void BufferToInputPacket();

private:
	sf::RenderWindow* window_;

	void onNotify(Message message)
	{
		int bufferOffset = 0;

		// Client ID
		uint16_t u16;
		memcpy(&u16, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
		bufferOffset += sizeof(uint16_t);
		std::cout << "Client ID: " << u16 << std::endl;

		// Type/ Sub-Type
		memcpy(&u16, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
		bufferOffset += sizeof(uint16_t);
		std::cout << "Type: " << u16 << std::endl;
		memcpy(&u16, message.getMessage().c_str() + bufferOffset, sizeof(uint16_t));
		bufferOffset += sizeof(uint16_t);
		std::cout << "Sub-Type: " << u16 << std::endl;

		//
		InputPacket newInputPacket;
		newInputPacket.left = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
		newInputPacket.right = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
		newInputPacket.up = (uint8_t)message.getMessage().c_str() + ++bufferOffset;
		newInputPacket.down = (uint8_t)message.getMessage().c_str() + ++bufferOffset;

		// REMOVE
		if(newInputPacket.left)
			std::cout << "left" << std::endl;
		else if (newInputPacket.right)
			std::cout << "right" << std::endl;
		else if (newInputPacket.up)
			std::cout << "up" << std::endl;
		else if (newInputPacket.down)
			std::cout << "down" << std::endl;
		
		//switch message type
		//{
		//0:
		//	turn to input packet

		//	add to input buffer
		//break;
		//}
	}
};

