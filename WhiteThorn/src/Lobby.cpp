#include "Lobby.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderLayer2D.h"

using namespace BlackThorn;

#define BIND_E_FN(x) std::bind(&Lobby::x, this, std::placeholders::_1)

void Lobby::Init()
{
	Application& app = Application::Get();
	if (app.IsNetNull() == false && app.GetNetwork().IsServer())
	{
		app.GetNetwork().Bind(NetworkCallBack::ConnectionReceived, BIND_E_FN(PlayerConnected));
	}

	m_Player = RenderLayer2D::Get().GetPlayer();
	m_NetworkEntities = RenderLayer2D::Get().GetNetworkPlayers();
}

void Lobby::OnUpdate(BlackThorn::Timestep ts)
{
	m_Player->OnUpdate(ts);
}

void Lobby::OnRender()
{
	m_Player->OnRender();

	for (auto const& entity : m_NetworkEntities) {
		entity->OnRender();
	}
}

void Lobby::OnImGuiRender()
{

}

void Lobby::PlayerConnected(int x)
{
	Entity* newPlayer = new Entity();
	newPlayer->ChangeTexture(Texture2D::Create("assets/sprites/classes/wizard_spritesheet.png", 32, 32));
	newPlayer->SetPosition(SpawnPoints[1]);
	m_NetworkEntities.push_back(newPlayer);
}
