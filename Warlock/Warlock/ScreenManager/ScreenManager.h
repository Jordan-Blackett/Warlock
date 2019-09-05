#pragma once

#include "Screen.h"
#include "GameScreen.h"

//#include <cstdint>

#include "../MessagingSystem/BusNode.h"

class ScreenManager : public BusNode
{
public:
	~ScreenManager() {};
	static ScreenManager* GetInstance();

	void Initialize(MessagingSystem* messageBus, sf::RenderWindow* Window);
	void Update();
	void Render();

	//void SendMessage(std::string Message);
	//std::string CreatePacket(uint16_t clientID, uint16_t packetType, uint16_t packetSubType, std::string message);
	//void MNotify();

private:
	ScreenManager() {};
	ScreenManager(ScreenManager const&) {};
	ScreenManager& operator=(ScreenManager const&) {};

	void OnNotify(Message& message);

	static ScreenManager* m_Instance;
	Screen* m_CurrentScreen;
};

