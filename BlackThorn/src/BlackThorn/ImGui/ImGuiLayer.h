#pragma once

#include "BlackThorn/Core/Layer.h"

#include "BlackThorn/Events/ApplicationEvent.h"
#include "BlackThorn/Events/KeyEvent.h"
#include "BlackThorn/Events/MouseEvent.h"

namespace BlackThorn {

	class BLACKTHORN_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}

