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


void ScreenManager::Initialize(sf::RenderWindow * Window)
{
	currentScreen = new GameScreen();
	currentScreen->Initialize(Window);
}

void ScreenManager::Update()
{
	currentScreen->Update();
}

void ScreenManager::Render()
{
	currentScreen->Render();
}