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

void dae::Collider::CheckCollisions(std::vector<dae::Collider*>* colliders)
{
	if (!m_look)
		return;

	glm::vec2 pos = GetPosition();
	for (int i = 0; i < (int)colliders->size(); i++)
	{
		Collider* coll = colliders->at(i);
		if (coll->GetSeen() && coll->GetOwner() != GetOwner())//can be looked at and has things to look at
		{
			coll->GetPosition();
			bool isColliding = (m_RealPos.x < coll->m_RealPos.x + coll->m_Dimensions.x &&
				m_RealPos.x + m_Dimensions.x > coll->m_RealPos.x &&
				m_RealPos.y < coll->m_RealPos.y + coll->m_Dimensions.y &&
				m_RealPos.y + m_Dimensions.y > coll->m_RealPos.y);

			//check if colliding
			if (isColliding)
			{
				if (!m_WasColliding.contains(coll))
				{
					m_WasColliding.emplace(coll, false);
				}

				if (m_WasColliding[coll] == false)
				{
					GetOwner()->CollisionEnter(coll, this);
				}
				GetOwner()->Collision(coll, this);
			}
			else
			{
				if (m_WasColliding.contains(coll) && m_WasColliding[coll] == true)
				{
					GetOwner()->CollisionExit(coll, this);
				}
			}

			m_WasColliding[coll] = isColliding;
		}
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