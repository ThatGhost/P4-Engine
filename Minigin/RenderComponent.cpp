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

void dae::RenderComponent::SetAnimationDetails(int MaxFrames, float FrameTime)
{
	m_FrameTime = FrameTime;
	m_MaxFrames = MaxFrames;
}

void dae::RenderComponent::SetRow(int row, int Frames)
{
	m_Row = row;
	if (Frames != -1)
		m_MaxFrames = Frames;
}

void dae::RenderComponent::Update(float deltaTime)
{
	if (m_IsAnimated)
	{
		if (m_Timer >= m_FrameTime)
		{
			m_Timer -= m_FrameTime;
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

	if(!m_IsAnimated)
		Renderer::GetInstance().RenderTexture(*m_Image.get(), position.x, position.y, ImageSize * m_Scale, ImageSize * m_Scale);
	else
	{
		SDL_Rect src{};
		src.x = (int)(ImageSize * m_Frame);
		src.y = (int)(ImageSize * m_Row);
		src.w = (int)(ImageSize);
		src.h = (int)(ImageSize);

		SDL_Rect dst{};
		dst.x = (int)(position.x);
		dst.y = (int)(position.y);
		dst.w = (int)(ImageSize * m_Scale);
		dst.h = (int)(ImageSize * m_Scale);

		Renderer::GetInstance().RenderTexture(*m_Image.get(),src,dst);
	}
}