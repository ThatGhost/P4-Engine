#include "MiniginPCH.h"
#include "Collider.h"
#include "EventManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::Collider::Collider(dae::GameObject* gameobject) : Component(gameobject)
{
	EventManager::AddEvent("0STARTBUTTON", std::bind(&dae::Collider::EnableDebug, this, m_Argument));
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonStart, "STARTBUTTON");
}

void dae::Collider::CheckCollisions()
{
	GetPosition();
	if (m_EventName == "")
		return;
	for (Collider* coll : m_CollidersToCheck)
	{
		coll->GetPosition();
		bool isColliding = (m_RealPos.x < coll->m_RealPos.x + coll->m_Dimensions.x &&
							m_RealPos.x + m_Dimensions.x > coll->m_RealPos.x &&
							m_RealPos.y < coll->m_RealPos.y + coll->m_Dimensions.y &&
							m_RealPos.y + m_Dimensions.y > coll->m_RealPos.y);

		//check if colliding
		if (isColliding)
		{
			EventManager::SendEvent(m_EventName, 0.f);
		}

		m_WasColliding = isColliding;
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
	if(m_Image == nullptr)
		m_Image = ResourceManager::GetInstance().LoadTexture("..\\Data\\DebugRectangle.png");
	m_DebugEnabled = !m_DebugEnabled;
}