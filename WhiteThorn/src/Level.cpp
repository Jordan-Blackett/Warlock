#include "Level.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace BlackThorn;

void Level::Init()
{
	m_Player.LoadAssets();
}

void Level::OnUpdate(BlackThorn::Timestep ts)
{
	m_Player.OnUpdate(ts);
}

void Level::OnRender()
{
	m_Player.OnRender();
}

void Level::OnImGuiRender()
{
	m_Player.OnImGuiRender();
}

void Level::GameOver()
{
	m_GameOver = true;
}

void Level::Reset()
{
	m_GameOver = false;

	//m_Player.Reset();
}