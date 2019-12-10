#pragma once

#include "Entity.h"

class Level
{
public:
	void Init();

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	bool IsGameOver() const { return m_GameOver; }
	void Reset();

	Entity& GetPlayer() { return m_Player; }
private:

	void GameOver();
private:
	Entity m_Player;

	bool m_GameOver = false;

	BlackThorn::Ref<BlackThorn::Texture2D> m_TriangleTexture;
};

