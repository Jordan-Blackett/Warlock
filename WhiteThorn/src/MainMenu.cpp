#include "MainMenu.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "RenderLayer2D.h"

using namespace BlackThorn;

#define BIND_E_FN(x) std::bind(&Lobby::x, this, std::placeholders::_1)

void MainMenu::Init()
{
	BlackThorn::Ref<BlackThorn::Texture2D> Wizard = Texture2D::Create("assets/sprites/classes/wizard_spritesheet.png", 32, 32);
	m_ClassTextures.push_back(Wizard);
	BlackThorn::Ref<BlackThorn::Texture2D> Cleric = Texture2D::Create("assets/sprites/classes/cleric_spritesheet.png", 32, 32);
	m_ClassTextures.push_back(Cleric);
	BlackThorn::Ref<BlackThorn::Texture2D> Ranger = Texture2D::Create("assets/sprites/classes/ranger_spritesheet.png", 32, 32);
	m_ClassTextures.push_back(Ranger);
	BlackThorn::Ref<BlackThorn::Texture2D> Rogue = Texture2D::Create("assets/sprites/classes/rogue_spritesheet.png", 32, 32);
	m_ClassTextures.push_back(Rogue);
	BlackThorn::Ref<BlackThorn::Texture2D> Warrior = Texture2D::Create("assets/sprites/classes/warrior_spritesheet.png", 32, 32);
	m_ClassTextures.push_back(Warrior);

	m_Player = RenderLayer2D::Get().GetPlayer();
	m_Player->ChangeTexture(Wizard);
}

void MainMenu::OnUpdate(BlackThorn::Timestep ts)
{
}

void MainMenu::OnRender()
{
	m_Player->OnRender();
}

void MainMenu::OnImGuiRender()
{
	ImGui::Begin("Network Settings");
	ImGui::Text("Character Settings:");
	char playerName[16] = { "Player" };
	ImGui::Text("Name:");
	ImGui::InputText("", playerName, IM_ARRAYSIZE(playerName));
	ImGui::Text("Select Class:");
	if (ImGui::Button("Wizard")) {
		m_Player->ChangeTexture(m_ClassTextures[0]);
	}
	if (ImGui::Button("Cleric")) {
		m_Player->ChangeTexture(m_ClassTextures[1]);
	}
	if (ImGui::Button("Ranger")) {
		m_Player->ChangeTexture(m_ClassTextures[2]);
	}
	if (ImGui::Button("Rogue")) {
		m_Player->ChangeTexture(m_ClassTextures[3]);
	}
	if (ImGui::Button("Warrior")) {
		m_Player->ChangeTexture(m_ClassTextures[4]);
	}
	ImGui::NewLine();
	ImGui::Text("Network Settings:");
	static char ipAddress[16] = { "127.0.0.1" };
	static char port[16] = { "5575" };
	ImGui::InputText("IP Address", ipAddress, IM_ARRAYSIZE(ipAddress));
	ImGui::InputText("Port", port, IM_ARRAYSIZE(port));
	Application& app = Application::Get();
	if (ImGui::Button("Host")) {
		if (app.GetNetwork().CreateServer(5575, 4))
		{
			RenderLayer2D::Get().ChangeLevel(GameState::Lobby);
		}
	}
	if (ImGui::Button("Join")) {
		if (app.GetNetwork().CreateClient(ipAddress, 5575))
		{
			// Send Character DATA RPC
			RenderLayer2D::Get().ChangeLevel(GameState::Lobby);
		}
	}
	ImGui::End();
}