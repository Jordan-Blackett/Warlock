#pragma once

#include "BlackThorn.h"

#include "MainMenu.h"
#include "Lobby.h"
#include "Level.h"

#include <imgui/imgui.h>

enum class GameState
{
	Play = 0, MainMenu = 1, Lobby = 2, GameOver = 3
};

class RenderLayer2D : public BlackThorn::Layer
{
public:
	RenderLayer2D();
	virtual ~RenderLayer2D();

	inline static RenderLayer2D& Get() { return *s_Instance; }

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(BlackThorn::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BlackThorn::Event& e) override;
	bool OnMouseButtonPressed(BlackThorn::MouseButtonPressedEvent& e);
	bool OnWindowResize(BlackThorn::WindowResizeEvent& e);

	void ChangeLevel(GameState state);

	//
	Entity* GetPlayer() { return &m_Player; }
	std::vector<Entity*> GetNetworkPlayers() { return m_NetworkEntities; }

private:
	void CreateCamera(uint32_t width, uint32_t height, float cameraWidth);

private:
	BlackThorn::Scope<BlackThorn::OrthographicCamera> m_Camera;

	// Levels
	MainMenu m_Menu;
	Lobby m_Lobby;
	Level m_Level;

	// Entities
	Entity m_Player;
	std::vector<Entity*> m_NetworkEntities;

	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	//enum class GameState
	//{
	//	Play = 0, MainMenu = 1, Lobby = 2, GameOver = 3
	//};

	GameState m_State = GameState::MainMenu;

private:
	static RenderLayer2D* s_Instance;
};
