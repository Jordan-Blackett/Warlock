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

	animated = true;

	Animation idle = Animation(4, 10, 0.1, false);
	Animation gesture = Animation(3, 10, 0.1, true);
	Animation walk = Animation(2, 10, 0.1, false);
	Animation attack = Animation(1, 10, 0.1, true);
	Animation dealth = Animation(0, 10, 0.1, true);

	animations.emplace("Idle", idle);
	animations.emplace("Gesture", gesture);
	animations.emplace("Walk", walk);
	animations.emplace("Attack", attack);
	animations.emplace("Dealth", dealth);

	Play("Idle");
}

void Entity::OnUpdate(BlackThorn::Timestep ts)
{
	m_Time += ts;

	m_Velocity = { 0.0f, 0.0f };

	animationStates = AnimationStates::Idle;

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

	if (Input::IsKeyPressed(BT_KEY_F))
	{
		animationStates = AnimationStates::Gesture;
		Play("Gesture");
		m_Velocity = { 0.0f, 0.0f };
	} 
	else if (Input::IsKeyPressed(BT_KEY_SPACE))
	{
		animationStates = AnimationStates::Attack;
		Play("Attack");
		m_Velocity = { 0.0f, 0.0f };
	}
	m_Position += m_Velocity * (float)ts;

	//Animation
	if (animationStates == AnimationStates::Idle)
	{
		if (m_Velocity.x > 0 || m_Velocity.x < 0
			|| m_Velocity.y > 0 || m_Velocity.y < 0) {
			Play("Walk");
		}
		else {
			Play("Idle");
		}
	}

	animDeltaTime += (float)ts;
	CurrentAnimationFrame = (static_cast<int>(animDeltaTime / AnimationSpeed) % AnimationFrames);

	if (currentRequiredFinish && CurrentAnimationFrame == (AnimationFrames - 1))
		CurrentAnimationFinished = true;
}

void Entity::OnRender()
{
	m_EntityTexture->Bind();
	//Renderer2D::DrawQuad({ m_Position.x, m_Position.y, 0.5f }, { 1.0f, 1.0f }, glm::radians(GetRotation()), m_EntityTexture, 0);
	Renderer2D::DrawQuad({ m_Position.x, m_Position.y }, { 1.0f, 1.0f }, m_EntityTexture, CurrentAnimationFrame + (AnimationIndex * 10));
}

void Entity::OnImGuiRender()
{
}

void Entity::Play(const char* animationName)
{
	if (animationName != currentAnimationName 
		&& (animations[currentAnimationName].m_RequiredFinish == CurrentAnimationFinished))
	{
		AnimationIndex = animations[animationName].m_Index;
		AnimationFrames = animations[animationName].m_Frames;
		AnimationSpeed = animations[animationName].m_Speed;
		currentAnimationName = (char*)animationName;
		CurrentAnimationFrame = 0;
		animDeltaTime = 0;
		CurrentAnimationFinished = false;
		if (animations[animationName].m_RequiredFinish)
			currentRequiredFinish = true;
		else
			currentRequiredFinish = false;
	}
}
