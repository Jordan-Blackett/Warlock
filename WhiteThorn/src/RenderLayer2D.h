#pragma once

#include "BlackThorn.h"

#include "Level.h"
#include <imgui/imgui.h>

class RenderLayer2D : public BlackThorn::Layer
{
public:
	RenderLayer2D();
	virtual ~RenderLayer2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(BlackThorn::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(BlackThorn::Event& e) override;
	bool OnMouseButtonPressed(BlackThorn::MouseButtonPressedEvent& e);
	bool OnWindowResize(BlackThorn::WindowResizeEvent& e);

private:
	void CreateCamera(uint32_t width, uint32_t height);

private:
	BlackThorn::Scope<BlackThorn::OrthographicCamera> m_Camera;
	Level m_Level;
	ImFont* m_Font;
	float m_Time = 0.0f;
	bool m_Blink = false;

	enum class GameState
	{
		Play = 0, MainMenu = 1, GameOver = 2
	};

	GameState m_State = GameState::MainMenu;
};
