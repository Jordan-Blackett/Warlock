#include "ScreenManager.h"

// Singleton
ScreenManager* ScreenManager::GetInstance()
{
	static ScreenManager instance;
	return &instance;
}

ScreenManager::ScreenManager()
{
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::Initialize(Client_MessagingSystem* messageBus, sf::RenderWindow * Window)
{
	Init(messageBus);

	currentScreen = new GameScreen();
	currentScreen->Initialize(Window);
}

void ScreenManager::Update()
{
	Notify();
	currentScreen->Update();
}

void ScreenManager::Render()
{
	currentScreen->Render();
}

void ScreenManager::onNotify(Message message)
{
	// Consumer

	//currentScreen->onNotify(message);
}

void ScreenManager::SendMessage(std::string NewMessage)
{
	Message msg(NewMessage);
	SendMessageSystem(msg);
}

std::string ScreenManager::CreatePacket(uint16_t clientID, uint16_t packetType, uint16_t packetSubType, std::string message)
{
	char packetBuffer[128];

	uint16_t clientIDHtoN = htons(clientID);
	uint16_t packetTypeHtoN = htons(packetType);
	uint16_t packetSubTypeHtoN = htons(packetSubType);

	// Header
	memcpy(packetBuffer, &clientIDHtoN, sizeof(uint16_t));
	memcpy(packetBuffer + 2, &packetTypeHtoN, sizeof(uint16_t));
	memcpy(packetBuffer + 4, &packetSubTypeHtoN, sizeof(uint16_t));

	// Message
	if (message != "")
	{
		strcpy_s(packetBuffer + 6, sizeof(packetBuffer), message.c_str());
	}

	return std::string(packetBuffer, 6);
}

void ScreenManager::MNotify()
{
	Notify();
}
