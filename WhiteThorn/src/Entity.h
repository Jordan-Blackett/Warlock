#pragma once

#include <BlackThorn.h>

class Entity
{
public:
	Entity();

	void LoadAssets();

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	const glm::vec2& GetPosition() const { return m_Position; }
	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }

private:
	glm::vec2 m_Position = { 0.0f, 0.0f };
	glm::vec2 m_Velocity = { 0.0f, 0.0f };

	float m_MovementSpeed = 2.5f;

	float m_Time = 0.0f;

	BlackThorn::Ref<BlackThorn::Texture2D> m_EntityTexture;
};