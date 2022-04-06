#include "MiniginPCH.h"
#include "Collider.h"
#include "EventManager.h"

dae::Collider::Collider(dae::GameObject* gameobject) : Component(gameobject)
{

}

void dae::Collider::CheckCollisions()
{
	GetPosition();
	for (Collider* coll : m_CollidersToCheck)
	{
		coll->GetPosition();
		bool isColliding = (m_RealPos.x < coll->m_RealPos.y + coll->m_Dimensions.x &&
			m_RealPos.x + m_Dimensions.x > coll->m_RealPos.y &&
			m_RealPos.y < coll->m_RealPos.y + coll->m_Dimensions.y &&
			m_RealPos.y + m_Dimensions.y > coll->m_RealPos.y);

		//check if colliding
		if (isColliding && !m_WasColliding)
			EventManager::SendEvent(m_EventNameEnter,0);

		if (!isColliding && m_WasColliding)
			EventManager::SendEvent(m_EventNameExit,0);

		m_WasColliding = isColliding;
	}
}

glm::vec2 dae::Collider::GetPosition()
{
	GameObject* owner = GetOwner();
	m_RealPos = m_Position + owner->GetPosition().GetPosition();
	return m_RealPos;
}