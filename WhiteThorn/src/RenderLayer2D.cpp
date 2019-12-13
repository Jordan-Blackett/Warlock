#include "RenderLayer2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace BlackThorn;

RenderLayer2D* RenderLayer2D::s_Instance = nullptr;

RenderLayer2D::RenderLayer2D()
	: Layer("RenderLayer2D")
{
	BT_CORE_ASSERT(!s_Instance, "RenderLayer2D already axists!");
	s_Instance = this;

	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight(), 1.0f);

	Application& app = Application::Get();
	app.SetNetwork(new Network());
	app.GetNetwork().Initialize();
}


RenderLayer2D::~RenderLayer2D()
{
}

void RenderLayer2D::OnAttach()
{
	//ImGuiIO io = ImGui::GetIO();
	//m_Font = io.Fonts->AddFontFromFileTTF("assets/OpenSans-Regular.ttf", 20.0f); //120.0f

	m_Menu.Init();
}

void RenderLayer2D::OnDetach()
{
}

void RenderLayer2D::OnUpdate(BlackThorn::Timestep ts)
{
	// Update
	m_Time += ts;
	if ((int)(m_Time * 10.0f) % 8 > 4)
		m_Blink = !m_Blink;

	if (m_Level.IsGameOver())
		m_State = GameState::GameOver;

	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });

	switch (m_State)
	{
		case GameState::MainMenu:
		{
			m_Menu.OnUpdate(ts);
			break;
		}
		case GameState::Lobby:
		{
			m_Lobby.OnUpdate(ts);
			break;
		}
		case GameState::Play:
		{
			break;
		}
	}

	// Render
	BlackThorn::RenderCommand::SetClearColor({ 0.0f, 0.0f, 1.0f, 1 });
	BlackThorn::RenderCommand::Clear();

	BlackThorn::Renderer2D::BeginScene(*m_Camera);

	switch (m_State)
	{
		case GameState::MainMenu:
		{
			m_Menu.OnRender();
			break;
		}
		case GameState::Lobby:
		{
			m_Lobby.OnRender();
			break;
		}
		case GameState::Play:
		{
			break;
		}
		case GameState::GameOver:
		{
			break;
		}
	}

	BlackThorn::Renderer2D::EndScene();
}

void RenderLayer2D::OnImGuiRender()
{
	char ipaddress[BUFFER_SIZE];

	Application& app = Application::Get();
	if (app.GetNetwork().IsActive())
	{
		char m_IPAddress[16] = { "127.0.0.1" };
		char m_Port[16] = { "5575" };

		// Overlay
		ImGui::Begin("Network Stats");
		if(app.GetNetwork().IsServer())
			ImGui::Text("Host");
		else
			ImGui::Text("Client");
		
		ImGui::Text("IP Address %s", m_IPAddress);
		ImGui::Text("Port %s", m_IPAddress);

		//std::map<u_int64, SOCKET>& test = app.GetNetwork().GetClientConnects();
		////ImGui::Text("Players - %s", 2);

		//for (auto const& [key, val] : test)
		//{
		//	ImGui::Text("%s (%s)", key, val);
		//}

		char name[16] = { "Jordan" };


		ImGui::End();
	}

	switch (m_State)
	{
		case GameState::MainMenu:
		{
			m_Menu.OnImGuiRender();
			break;
		}
		case GameState::Lobby:
		{
			m_Lobby.OnImGuiRender();
			break;
		}
		case GameState::Play:
		{
			break;
		}
		case GameState::GameOver:
		{
			break;
		}
	}
}

void RenderLayer2D::OnEvent(BlackThorn::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(BT_BIND_EVENT_FN(RenderLayer2D::OnWindowResize));
	dispatcher.Dispatch<MouseButtonPressedEvent>(BT_BIND_EVENT_FN(RenderLayer2D::OnMouseButtonPressed));
}

bool RenderLayer2D::OnMouseButtonPressed(BlackThorn::MouseButtonPressedEvent& e)
{
	if (m_State == GameState::GameOver)
		m_Level.Reset();

	//m_State = GameState::Play;
	return false;
}

bool RenderLayer2D::OnWindowResize(BlackThorn::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight(), 1.0f);
	return false;
}

void RenderLayer2D::ChangeLevel(GameState state)
{
	switch (state)
	{
		case GameState::MainMenu:
		{
			break;
		}
		case GameState::Lobby:
		{
			m_State = GameState::Lobby;
			m_Lobby.Init();
			auto& window = Application::Get().GetWindow();
			CreateCamera(window.GetWidth(), window.GetHeight(), 5.0f);
			break;
		}
		case GameState::Play:
		{
			break;
		}
		case GameState::GameOver:
		{
			break;
		}
	}
}

void RenderLayer2D::CreateCamera(uint32_t width, uint32_t height, float cameraWidth)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = cameraWidth;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}