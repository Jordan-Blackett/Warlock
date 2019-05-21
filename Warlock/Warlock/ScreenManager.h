#pragma once

#include "Screen.h"

#include "GameScreen.h"
#include <WinSock2.h>

#include "Client_BusNode.h"

class ScreenManager : public Client_BusNode
{
public:
	~ScreenManager();
	static ScreenManager* GetInstance();

	void Initialize(Client_MessagingSystem* messageBus, sf::RenderWindow* Window);
	void Update();
	void Render();

	void SendMessage(std::string Message);
	std::string CreatePacket(uint16_t clientID, uint16_t packetType, uint16_t packetSubType, std::string message);
	void MNotify();

private:
	ScreenManager(); // Private constructor to prevent instancing
	ScreenManager(ScreenManager const&) {};
	ScreenManager& operator=(ScreenManager const&) {};

	void onNotify(Message message);

	static ScreenManager* instance;
	Screen *currentScreen;
};

