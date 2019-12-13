#pragma once

#include "Entities/Entity.h"

class MainMenu
{
public:
	void Init();

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

private:
	Entity* m_Player;

	std::vector<BlackThorn::Ref<BlackThorn::Texture2D>> m_ClassTextures;
};

