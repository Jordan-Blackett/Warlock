#pragma once

#include "Entities/Entity.h"

class Lobby
{
public:
	void Init();

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	// Callbacks
	void PlayerConnected(int x);

private:
	Entity* m_Player;
	std::vector<Entity*> m_NetworkEntities;

	glm::vec2 SpawnPoints[6] = { { 0, 0 },  { 1, 0 }, { -1, 0 } };
};

