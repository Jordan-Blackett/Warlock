#pragma once

#include <BlackThorn.h>

class Entity
{
public:
	Entity();

	void LoadAssets(const std::string& path);
	void ChangeTexture(const BlackThorn::Ref<BlackThorn::Texture2D> texture);

	void OnUpdate(BlackThorn::Timestep ts);
	void OnRender();

	void OnImGuiRender();

	const glm::vec2& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec2& newPosition) { m_Position = newPosition; }
	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }

private:
	glm::vec2 m_Position = { 0.0f, 0.0f };
	glm::vec2 m_Velocity = { 0.0f, 0.0f };

	float m_MovementSpeed = 2.5f;

	float m_Time = 0.0f;

	BlackThorn::Ref<BlackThorn::Texture2D> m_EntityTexture;

	// Animation

	enum class AnimationStates {
		Idle = 0,
		Gesture = 1,
		Walk = 2,
		Attack = 3,
		Dealth = 4
	};

	AnimationStates animationStates;

	bool animated = false;

	void Play(const char* animationName);

	struct Animation {
		int m_Index;
		int m_Frames;
		float m_Speed;
		bool m_RequiredFinish;

		Animation() {}
		Animation(int index, int frames, float speed, bool finish)
		{
			m_Index = index;
			m_Frames = frames;
			m_Speed = speed;
			m_RequiredFinish = finish;
		}
	};

	std::map<const char*, Animation> animations;
	unsigned int AnimationIndex = 0;
	unsigned int AnimationFrames = 0;
	float AnimationSpeed = 0;

	float animDeltaTime = 0;
	char* currentAnimationName = { "Walk" };
	bool currentRequiredFinish = false;
	bool CurrentAnimationFinished = false;
	int CurrentAnimationFrame = 0;

};