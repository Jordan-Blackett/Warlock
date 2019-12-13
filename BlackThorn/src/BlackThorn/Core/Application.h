#pragma once

#include "Core.h"

#include "Window.h"
#include "BlackThorn/Core/LayerStack.h"
#include "BlackThorn/Events/Event.h"
#include "BlackThorn/Events/ApplicationEvent.h"

#include "BlackThorn/Core/TimeStep.h"

#include "BlackThorn/ImGui/ImGuiLayer.h"

#include "BlackThorn/Network/Network.h"

namespace BlackThorn {

	class BLACKTHORN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		inline Network& GetNetwork() { return *m_Network; }
		inline void SetNetwork(Network* network) { m_Network = network; }
		inline bool IsNetNull() {
			if (m_Network == nullptr)
				return true;
			else
				return false;
		}
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		Network* m_Network;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

	//inline Network* GetNetwork() { return Application::GetNetwork(); }
	//inline void SetNetwork(Network* network) { Application::SetNetwork(network); }

}