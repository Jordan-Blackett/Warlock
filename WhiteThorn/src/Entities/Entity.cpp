#include "Entity.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

using namespace BlackThorn;

Entity::Entity()
{
}

void Entity::LoadAssets(const std::string& path)
{
	m_EntityTexture = Texture2D::Create(path, 32, 32);
}

void Entity::ChangeTexture(const BlackThorn::Ref<BlackThorn::Texture2D> texture)
{
	m_EntityTexture = texture;
}

void Entity::OnUpdate(BlackThorn::Timestep ts)
{
	m_Time += ts;

	m_Velocity = { 0.0f, 0.0f };

	if (Input::IsKeyPressed(BT_KEY_W))
	{
		m_Velocity.y += m_MovementSpeed;
	}
	else if (Input::IsKeyPressed(BT_KEY_S))
	{
		m_Velocity.y -= m_MovementSpeed;
	}

	if (Input::IsKeyPressed(BT_KEY_A))
	{
		m_Velocity.x -= m_MovementSpeed;
	}
	else if (Input::IsKeyPressed(BT_KEY_D))
	{
		m_Velocity.x += m_MovementSpeed;
	}

	m_Position += m_Velocity * (float)ts;
}

void Entity::OnRender()
{
	m_EntityTexture->Bind();
	//Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.0f }, glm::radians(GetRotation()), m_EntityTexture, 0);
	Renderer2D::DrawQuad({ m_Position.x, m_Position.y }, { 1.0f, 1.0f }, m_EntityTexture, 0);
}

void Entity::OnImGuiRender()
{
}