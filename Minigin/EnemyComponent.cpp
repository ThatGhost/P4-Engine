#include "MiniginPCH.h"
#include "EnemyComponent.h"
#include "EnemySpawner.h"
#include "RenderComponent.h"
#include "Collider.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "PlayerController.h"
#include "BurgerComponent.h"
#include "GameManager.h"

dae::EnemyComponent::EnemyComponent(GameObject* owner) : Component(owner)
{
	m_Spawner = static_cast<EnemySpawner*>(GetOwner()->GetParent()->GetComponent<EnemySpawner>());
	m_Renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
	m_Player = SceneManager::GetInstance().FindComponent<PlayerController>()->GetOwner();
}

dae::EnemyComponent::~EnemyComponent()
{

}

void dae::EnemyComponent::Init(EnemyType type)
{
	m_type = type;
	switch (type)
	{
	case EnemyType::sausage:
		m_Score = 100;
		break;
	case EnemyType::pickle:
		m_Score = 200;
		break;
	case EnemyType::egg:
		m_Score = 300;
		break;
	default:
		break;
	}
}

void dae::EnemyComponent::Update(float deltaTime)
{
	//pepper
	if (m_Peppered)
	{
		if (m_PepperTimer >= m_PepperTime)
		{
			m_Peppered = false;
			m_PepperTimer = 0;
			switch (m_moveState)
			{
			case EnemyMoveState::Left: m_Renderer->SetRow(1); break;
			case EnemyMoveState::Right: m_Renderer->SetRow(2); break;
			case EnemyMoveState::Up:
			case EnemyMoveState::Down: m_Renderer->SetRow(3); break;
			default:
				break;
			}
		}
		else m_PepperTimer += deltaTime;
		return;
	}

	//switch direction
	if (m_Reset && m_CanDown && m_CanUp && m_CanLeft && m_CanRight)
	{
		DecideDirection();
		m_Reset = false;
	}
	else if (!m_Reset)
	{
		if (m_OnPlatform && !m_CanDown)
		{
			m_Reset = true;
		}
		else if (!m_OnPlatform && !m_CanLeft)
		{
			m_Reset = true;
		}
	}

	glm::vec3 direction{};

	//!!look out and edge!! turn around
	if (!m_CanLeft && m_moveState == EnemyMoveState::Left) m_moveState = EnemyMoveState::Right;
	if (!m_CanRight && m_moveState == EnemyMoveState::Right) m_moveState = EnemyMoveState::Left;
	if (!m_CanUp && m_moveState == EnemyMoveState::Up) m_moveState = EnemyMoveState::Down;
	if (!m_CanDown && m_moveState == EnemyMoveState::Down) m_moveState = EnemyMoveState::Up;

	//movedir
	if (m_OnPlatform)
	{
		if (m_moveState == EnemyMoveState::Left) direction.x = -1;
		if (m_moveState == EnemyMoveState::Right) direction.x = 1;
	}
	else
	{
		if (m_moveState == EnemyMoveState::Down) direction.y = 1;
		if (m_moveState == EnemyMoveState::Up) direction.y = -1;
	}

	//speed
	direction.x *= deltaTime * m_Speed;
	direction.y *= deltaTime * m_Speed;

	GetOwner()->Move(direction);

	//reset
	m_CanDown = false;
	m_CanUp = false;
	m_CanLeft = false;
	m_CanRight = false;
}

void dae::EnemyComponent::Die()
{
	m_Spawner->RemoveEnemy(m_type);
	SceneManager::GetInstance().GetGameManager()->AddScore(m_Score);
	GetOwner()->Destroy();
}

void dae::EnemyComponent::Salt()
{

}

void dae::EnemyComponent::DecideDirection()
{
	glm::vec3 mypos{GetOwner()->GetPosition().GetPosition()};
	glm::vec3 playerpos{m_Player->GetPosition().GetPosition()};

	if (!m_OnPlatform)
	{
		if (mypos.x == playerpos.x)
		{}
		else if (mypos.x > playerpos.x)
		{
			m_moveState = EnemyMoveState::Left;
			SnapToPlatform();
			m_Renderer->SetRow(1);
			m_OnPlatform = true;
		}
		else
		{
			m_moveState = EnemyMoveState::Right;
			SnapToPlatform();
			m_Renderer->SetRow(2);
			m_OnPlatform = true;
		}
	}
	else
	{
		if (mypos.y == playerpos.y)
		{}
		else if (mypos.y > playerpos.y)
		{
			m_moveState = EnemyMoveState::Up;
			SnapToStair();
			m_Renderer->SetRow(3);
			m_OnPlatform = false;
		}
		else
		{
			m_moveState = EnemyMoveState::Down;
			SnapToStair();
			m_Renderer->SetRow(3);
			m_OnPlatform = false;
		}
	}
}

void dae::EnemyComponent::SnapToStair()
{
	glm::vec3 pos = GetOwner()->GetPosition().GetPosition();
	pos.x = m_StairPosition;
	GetOwner()->SetPosition(pos);
}

void dae::EnemyComponent::SnapToPlatform()
{
	glm::vec3 pos = GetOwner()->GetPosition().GetPosition();
	pos.y = m_PlatformPosition;
	GetOwner()->SetPosition(pos);
}

void dae::EnemyComponent::OnCollisionEnter(Collider* other, Collider* mine)
{
	if (mine->GetTag() == "ENEMY" && other->GetTag() == "PLAYER") //player interaction
	{
		if(!m_Peppered)
			EventManager::SendEvent("DIE");
		return;
	}
	if (mine->GetTag() == "ENEMY" && other->GetTag() == "BURGER") //burger interaction
	{
		dae::BurgerComponent* burger = static_cast<BurgerComponent*>(other->GetOwner()->GetComponent<dae::BurgerComponent>());
		if (burger->IsFalling())
			Die();
		return;
	}
	if (mine->GetTag() == "ENEMY" && other->GetTag() == "PEPPER") //burger interaction
	{
		m_Peppered = true;
		m_Renderer->SetRow(0);
		return;
	}
}

void dae::EnemyComponent::OnCollision(Collider* other, Collider* mine)
{
	if (mine->GetTag() == "BOT" && other->GetTag() == "STAIR") 
	{
		m_CanDown = true; 
		m_StairPosition = other->GetOwner()->GetPosition().GetPosition().x; 
	}
	else if (mine->GetTag() == "TOP" && other->GetTag() == "STAIR") { m_CanUp = true; m_StairPosition = other->GetOwner()->GetPosition().GetPosition().x;}
	else if (mine->GetTag() == "LEFT" && other->GetTag() == "PLATFORM") { m_CanLeft = true; m_PlatformPosition = other->GetOwner()->GetPosition().GetPosition().y -16; }
	else if (mine->GetTag() == "RIGHT" && other->GetTag() == "PLATFORM") { m_CanRight = true; m_PlatformPosition = other->GetOwner()->GetPosition().GetPosition().y -16; }
}
