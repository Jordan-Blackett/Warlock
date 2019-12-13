#include <BlackThorn.h>
#include <BlackThorn/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderLayer2D.h"

#include "Entities/Entity.h"

class NetworkLayer : public BlackThorn::Layer
{
public:
	NetworkLayer()
		: Layer("NetworkLayer")
	{
		// Network
		//if (BlackThorn::Network::Initialize())
		//{
		//	BT_CORE_WARN("Initialized Network!");

		//	//m_TCPListenerThread = std::thread(BlackThorn::Network::InitializeTCP, IP, PORT);
		//	//m_UDPThread = std::thread(BlackThorn::Network::InitializeUDP, IP, PORT);
		//}
	}

	virtual ~NetworkLayer() override
	{
		//BlackThorn::Network::CleanUp();
		//m_TCPListenerThread.join();
		//m_UDPThread.join();
		//std::terminate(m_TCPListenerThread);
	}

	virtual void OnUpdate(BlackThorn::Timestep ts) override
	{
	}

	virtual void OnImGuiRender() override
	{
	}

private:
	std::thread m_TCPListenerThread;
	std::thread m_UDPThread;
};

class RenderLayer : public BlackThorn::Layer
{
public:
	RenderLayer()
		: Layer("RenderLayer"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = BlackThorn::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};

		BlackThorn::Ref<BlackThorn::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(BlackThorn::VertexBuffer::Create(vertices, sizeof(vertices)));
		BlackThorn::BufferLayout layout = {
			{ BlackThorn::ShaderDataType::Float3, "a_Position" },
			{ BlackThorn::ShaderDataType::Float4, "a_Color" },
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		BlackThorn::Ref<BlackThorn::IndexBuffer> indexBuffer;
		indexBuffer.reset(BlackThorn::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVertexArray = BlackThorn::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		BlackThorn::Ref<BlackThorn::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(BlackThorn::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVertexBuffer->SetLayout({
			{ BlackThorn::ShaderDataType::Float3, "a_Position" },
			{ BlackThorn::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		BlackThorn::Ref<BlackThorn::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(BlackThorn::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1);
				color = v_Color;
			}
		)";

		m_Shader = BlackThorn::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1);
			}
		)";

		m_FlatColorShader = BlackThorn::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		m_Texture = BlackThorn::Texture2D::Create("assets/textures/Checkerboard.png");
		m_BlendTexture = BlackThorn::Texture2D::Create("assets/textures/blend_test.png");

		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnUpdate(BlackThorn::Timestep ts) override
	{
		//BT_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

		// Update
		m_CameraController.OnUpdate(ts);

		// Render
		BlackThorn::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		BlackThorn::RenderCommand::Clear();

		BlackThorn::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<BlackThorn::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				BlackThorn::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		BlackThorn::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_BlendTexture->Bind();
		BlackThorn::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.5f), glm::vec3(1.5f)));

		//BlackThorn::Renderer::Submit(m_flatColorShader, m_SquareVertexArray);
		//BlackThorn::Renderer::Submit(m_Shader, m_VertexArray);

		BlackThorn::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(BlackThorn::Event& e) override
	{
		m_CameraController.OnEvent(e);
		//BlackThorn::EventDispatcher dispatcher(event);
		//dispatcher.Dispatch<BlackThorn::KeyPressedEvent>(BT_BIND_EVENT_FN(RenderLayer::OnKeyPressedEvent));
	}

	//bool OnKeyPressedEvent(BlackThorn::KeyPressedEvent& event)
	//{
	//	if (event.GetKeyCode() == BT_KEY_LEFT)
	//		m_CameraPosition.x -= m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_RIGHT)
	//		m_CameraPosition.x += m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_UP)
	//		m_CameraPosition.y += m_CameraSpeed;

	//	if (event.GetKeyCode() == BT_KEY_DOWN)
	//		m_CameraPosition.y -= m_CameraSpeed;

	//	return false;
	//}

private:
	BlackThorn::ShaderLibrary m_ShaderLibrary;
	BlackThorn::Ref<BlackThorn::Shader> m_Shader;
	BlackThorn::Ref<BlackThorn::VertexArray> m_VertexArray;

	BlackThorn::Ref<BlackThorn::Shader> m_FlatColorShader;
	BlackThorn::Ref<BlackThorn::Texture2D> m_Texture, m_BlendTexture;

	BlackThorn::Ref<BlackThorn::VertexArray> m_SquareVertexArray;

	BlackThorn::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class WhiteThorn : public BlackThorn::Application
{
public:
	WhiteThorn()
	{
		PushLayer(new NetworkLayer());
		//PushLayer(new RenderLayer());
		PushLayer(new RenderLayer2D());
	}

	~WhiteThorn()
	{
	}

};

BlackThorn::Application* BlackThorn::CreateApplication()
{
	return new WhiteThorn();
}