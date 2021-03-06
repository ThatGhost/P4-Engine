#include "MiniginPCH.h"
#include "BurgerComponent.h"
#include "RenderComponent.h"
#include "EventManager.h"
#include "GameManager.h"
#include "Collider.h"

dae::BurgerComponent::BurgerComponent(GameObject* Owner) : Component(Owner)
{
	
}

dae::BurgerComponent::~BurgerComponent()
{

}

void dae::BurgerComponent::Update(float deltaTime)
{
	if (m_IsFalling)
	{
		GetOwner()->Move(glm::vec3(0, m_FallVelocity,0));
		m_FallVelocity += m_FallSpeed * deltaTime;
		if (m_FallVelocity > m_MaxSpeed)m_FallVelocity = m_MaxSpeed;
	}
}

void dae::BurgerComponent::OnCollisionEnter(Collider* other, Collider*)
{
	std::string otherTag{ other->GetTag() };
	if (otherTag == "PLATFORM")
	{
		m_FallVelocity = 0;
		m_IsFalling = false;
		for (size_t j = 0; j < sizeof(m_WalkSpots); j++)
		{
			m_WalkSpots[j] = false;
		}
		AddScore(50);
		return;
	}
	else if (otherTag == "HOLDER")
	{
		m_FallVelocity = 0;
		m_IsFalling = false;
		m_Done = true;
		EventManager::SendEvent("BURGERDONE");
		return;
	}
	else if (otherTag == "BURGER")
	{
		//m_isFalling = true;
		if (!m_Done)
		{
			bool done = static_cast<dae::BurgerComponent*>(other->GetOwner()->GetComponent<dae::BurgerComponent>())->IsDone();
			if (done)
			{
				m_IsFalling = false;
				m_FallVelocity = 0;
				m_Done = true;
				EventManager::SendEvent("BURGERDONE");
				AddScore(50);
			}
			else
			{
				m_IsFalling = true;
			}
		}
		return;
	}
}

void dae::BurgerComponent::OnCollision(Collider* other, Collider* mine)
{
	if (other->GetTag() == "PLAYER" && mine->GetTag() == "BURGER")
	{
		//position to [0,1]
		glm::vec2 myPos = mine->GetOwner()->GetPosition().GetPosition();
		glm::vec2 otherPos = other->GetOwner()->GetPosition().GetPosition();
		glm::vec2 myDimensions = mine->GetDimensions();
		glm::vec2 otherDimensions = other->GetDimensions();

		otherPos.x += otherDimensions.x / 2;
		float percentage = (otherPos.x-myPos.x) / myDimensions.x;

		//position in 5 quadrants
		int index = (int)(percentage * 5);
		index = glm::clamp(index,0,4);

		m_WalkSpots[index] = true;

		//check if it ran over everything
		bool canFall = true;
		for (size_t j = 0; j < sizeof(m_WalkSpots); j++)
		{
			if (m_WalkSpots[j] == false)
				canFall = false;
		}
		m_IsFalling = canFall;
	}
}

void dae::BurgerComponent::Init(bool bread)
{
	if (!bread)
	{
		RenderComponent* ren = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
		int r = rand()%5;
		switch (r)
		{
		case 0: ren->SetImage("Burger.png",true ,2); break;
		case 1: ren->SetImage("Cheese.png",true ,2); break;
		case 2: ren->SetImage("Salad.png",true  ,2); break;
		case 3: ren->SetImage("Tomatos.png",true,2); break;
		default:
			break;
		}
	}
}

void dae::BurgerComponent::AddScore(int score)
{
	if (m_Init)
	{
		m_Init = false;
		return;
	}
	SceneManager::GetInstance().GetGameManager()->AddScore(score);
	//TODO add scores for when enemys are on it
}
