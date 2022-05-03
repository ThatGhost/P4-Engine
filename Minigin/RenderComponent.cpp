#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::RenderComponent::RenderComponent(GameObject* owner) : Component(owner)
{}

void dae::RenderComponent::SetImage(const std::string& Image, bool IsAnimated, float scale)
{
	m_IsAnimated = IsAnimated;
	m_Image = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\"+Image);
	m_Scale = scale;
}

void dae::RenderComponent::SetAnimationDetails(int MaxFrames, float FrameTime, float scale)
{
	m_FrameTime = FrameTime;
	m_MaxFrames = MaxFrames;
	m_Scale = scale;
}

void dae::RenderComponent::Update(float deltaTime)
{
	if (m_IsAnimated)
	{
		if (m_Timer >= m_FrameTime)
		{
			m_Timer = 0;
			m_Frame++;
			if (m_Frame == m_MaxFrames)
				m_Frame = 0;
		}
		else
			m_Timer += deltaTime;
	}
}

void dae::RenderComponent::Render() const
{
	const int ImageSize = 32;

	glm::vec3 position = GetOwner()->GetPosition().GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Image.get(), position.x, position.y, ImageSize * m_Scale, ImageSize * m_Scale);
}