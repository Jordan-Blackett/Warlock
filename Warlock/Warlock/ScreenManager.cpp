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