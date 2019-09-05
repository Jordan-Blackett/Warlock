#include "ScreenManager.h"



ScreenManager* ScreenManager::m_Instance = nullptr;

// Singleton
ScreenManager* ScreenManager::GetInstance()
{
	if (!m_Instance)
		m_Instance = new ScreenManager();

	return m_Instance;
}

void ScreenManager::Initialize(MessagingSystem* messageBus, sf::RenderWindow * Window)
{
	BusNode::BusNode(messageBus);

	m_CurrentScreen = new GameScreen();
	m_CurrentScreen->Initialize(Window);
}

void ScreenManager::Update()
{
	//Notify();
	m_CurrentScreen->Update();
}

void ScreenManager::Render()
{
	m_CurrentScreen->Render();
}

void ScreenManager::OnNotify(Message& message)
{
	m_CurrentScreen->OnNotify(message);
}

//void ScreenManager::SendMessage(std::string NewMessage)
//{
	//Message msg(NewMessage);
	//SendMessageSystem(msg);
//}

//std::string ScreenManager::CreatePacket(uint16_t clientID, uint16_t packetType, uint16_t packetSubType, std::string message)
//{
//	char packetBuffer[128];
//
//	uint16_t clientIDHtoN = htons(clientID);
//	uint16_t packetTypeHtoN = htons(packetType);
//	uint16_t packetSubTypeHtoN = htons(packetSubType);
//
//	// Header
//	memcpy(packetBuffer, &clientIDHtoN, sizeof(uint16_t));
//	memcpy(packetBuffer + 2, &packetTypeHtoN, sizeof(uint16_t));
//	memcpy(packetBuffer + 4, &packetSubTypeHtoN, sizeof(uint16_t));
//
//	// Message
//	if (message != "")
//	{
//		strcpy_s(packetBuffer + 6, sizeof(packetBuffer), message.c_str());
//	}
//
//	return std::string(packetBuffer, 6);
//}

//void ScreenManager::MNotify()
//{
//	Notify();
//}
