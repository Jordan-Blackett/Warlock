#pragma once

#include "Screen.h"

#include "GameScreen.h"

class ScreenManager
{
public:
	~ScreenManager();
	static ScreenManager* GetInstance();

	void Initialize(sf::RenderWindow* Window);
	void Update();
	void Render();

private:
	ScreenManager(); // Private constructor to prevent instancing
	ScreenManager(ScreenManager const&) {};
	ScreenManager& operator=(ScreenManager const&) {};

	static ScreenManager* instance;
	Screen *currentScreen;

};

