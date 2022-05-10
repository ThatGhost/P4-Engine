#include "MiniginPCH.h"
#include "BurgerComponent.h"
#include "GameManager.h"
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

	if (m_ScoreToAdd > 0)
	{
		GameManager::GetInstance()->AddScore(m_ScoreToAdd);
		m_ScoreToAdd = 0;
	}
}

void dae::BurgerComponent::OnCollisionEnter(Collider* other, Collider*)
{
	std::string otherTag{ other->GetTag() };
	if (otherTag == "PLATFORM")
	{
		m_fallVelocity = 0;
		m_isFalling = false;
		for (size_t j = 0; j < sizeof(m_walkSpots); j++)
		{
			m_walkSpots[j] = false;
		}
		AddScore(50);
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

void dae::BurgerComponent::OnCollision(Collider* other, Collider* mine)
{
	if (other->GetTag() == "PLAYER")
	{
		//position to [0,1]
		glm::vec2 myPos = mine->GetOwner()->GetPosition().GetPosition();
		glm::vec2 otherPos = other->GetOwner()->GetPosition().GetPosition();
		glm::vec2 myDimensions = mine->GetDimensions();
		glm::vec2 otherDimensions = other->GetDimensions();

		otherPos.x += otherDimensions.x / 2;
		float percentage = (otherPos.x-myPos.x) / myDimensions.x;
		int index = (int)(percentage * 5);
		index = glm::clamp(index,0,4);

		m_walkSpots[index] = true;
		//check if it ran over everything
		bool canFall = true;
		for (size_t j = 0; j < sizeof(m_walkSpots); j++)
		{
			if (m_walkSpots[j] == false)
				canFall = false;
		}
		m_isFalling = canFall;
	}
}

void dae::BurgerComponent::AddScore(int score)
{
	if (m_ini)
	{
		m_ini = false;
		return;
	}
	m_ScoreToAdd += score;
	//TODO add scores for when enemys are on it
}
