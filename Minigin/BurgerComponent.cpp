#include "MiniginPCH.h"
#include "BurgerComponent.h"
#include "EventManager.h"
#include "Collider.h"

dae::BurgerComponent::BurgerComponent(GameObject* Owner) : Component(Owner)
{}

dae::BurgerComponent::~BurgerComponent()
{

}

void dae::BurgerComponent::Update(float deltaTime)
{
	if (m_isFalling)
	{
		GetOwner()->Move(glm::vec3(0, m_fallVelocity,0));
		m_fallVelocity += m_FallSpeed * deltaTime;
		if (m_fallVelocity > m_MaxSpeed)m_fallVelocity = m_MaxSpeed;
	}
}

void dae::BurgerComponent::OnCollisionEnter(Collider* other, Collider* mine)
{
	std::string otherTag{ other->GetTag() };
	if (otherTag == "PLAYER")
	{
		int i = std::stoi(mine->GetTag());
		m_walkSpots[i] = true;

		//check if it ran over everything
		bool canFall = true;
		for (size_t j = 0; j < sizeof(m_walkSpots); j++)
		{
			if (m_walkSpots[j] == false)
				canFall = false;
		}
		m_isFalling = canFall;
		return;
	}
	else if (otherTag == "PLATFORM")
	{
		m_fallVelocity = 0;
		m_isFalling = false;
		for (size_t j = 0; j < sizeof(m_walkSpots); j++)
		{
			m_walkSpots[j] = false;
		}
		return;
	}
	else if (otherTag == "HOLDER")
	{
		m_fallVelocity = 0;
		m_isFalling = false;
		m_done = true;
		return;
	}
	else if (otherTag == "BURGER")
	{
		m_isFalling = true;

		bool done = static_cast<dae::BurgerComponent*>(other->GetOwner()->GetComponent<dae::BurgerComponent>())->IsDone();
		if (done || m_done)
		{
			m_isFalling = false;
			m_fallVelocity = 0;
			m_done = true;
		}
		return;
	}
}