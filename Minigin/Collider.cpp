#include "MiniginPCH.h"
#include "Collider.h"
#include "EventManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::Collider::Collider(dae::GameObject* gameobject) : Component(gameobject)
{
	EventManager::AddEvent("0BUTTON_START", std::bind(&dae::Collider::EnableDebug, this, m_Argument));
}

void dae::Collider::CheckCollisions()
{
	GetPosition();
	for (int i = 0; i < (int)m_CollidersToCheck.size(); i++)
	{
		m_CollidersToCheck[i]->GetPosition();
		bool isColliding = (m_RealPos.x < m_CollidersToCheck[i]->m_RealPos.x + m_CollidersToCheck[i]->m_Dimensions.x &&
							m_RealPos.x + m_Dimensions.x > m_CollidersToCheck[i]->m_RealPos.x &&
							m_RealPos.y < m_CollidersToCheck[i]->m_RealPos.y + m_CollidersToCheck[i]->m_Dimensions.y &&
							m_RealPos.y + m_Dimensions.y > m_CollidersToCheck[i]->m_RealPos.y);

		//check if colliding
		if (isColliding)
		{
			if (m_Colliding[i] == false)
			{
				GetOwner()->CollisionEnter(m_CollidersToCheck[i],this);
			}
			GetOwner()->Collision(m_CollidersToCheck[i],this);
		}
		else
		{
			if (m_Colliding[i] == true)
			{
				GetOwner()->CollisionExit(m_CollidersToCheck[i], this);
			}
		}

		m_Colliding[i] = isColliding;
	}
}

glm::vec2 dae::Collider::GetPosition()
{
	GameObject* owner = GetOwner();
	m_RealPos = m_Position + owner->GetPosition().GetPosition();
	return m_RealPos;
}

void dae::Collider::Update(float)
{
	if(m_DebugEnabled)
		GetPosition();
}

void dae::Collider::Render() const 
{
	if (m_DebugEnabled)
	{
		Renderer::GetInstance().RenderTexture(*m_Image.get(), m_RealPos.x, m_RealPos.y, m_Dimensions.x, m_Dimensions.y);
	}
}

void dae::Collider::EnableDebug(float)
{
	if (m_Image == nullptr)
	{
		m_Image = ResourceManager::GetInstance().LoadTexture("..\\Data\\Sprites\\DebugRectangle.png");
		 
		if(m_Image != nullptr) m_DebugEnabled = !m_DebugEnabled;
	}
}