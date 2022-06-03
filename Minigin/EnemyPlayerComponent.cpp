#include "MiniginPCH.h"
#include "EnemyPlayerComponent.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Collider.h"
#include "RenderComponent.h"
#include "DestroyComponent.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "GameManager.h"

dae::EnemyPlayerComponent::EnemyPlayerComponent(GameObject* owner) : Component(owner)
{
	m_Startpos = GetOwner()->GetPosition().GetPosition();
	//input
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_LEFT", std::bind(&dae::EnemyPlayerComponent::Left, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_RIGHT", std::bind(&dae::EnemyPlayerComponent::Right, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_UP", std::bind(&dae::EnemyPlayerComponent::Up, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_DOWN", std::bind(&dae::EnemyPlayerComponent::Down, this));
	EventManager::AddEvent("DIE", std::bind(&dae::EnemyPlayerComponent::Restart, this));
}
dae::EnemyPlayerComponent::~EnemyPlayerComponent()
{
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_LEFT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_RIGHT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_UP");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_DOWN");
	EventManager::RemoveEvent("PEPPER");

	SoundManager::GetInstance().RemoveSound("Footstep.wav");
}

void dae::EnemyPlayerComponent::Update(float deltaTime)
{
	//pepper
	if (m_Peppered)
	{
		if (m_Timer >= m_PepperTime)
		{
			m_Peppered = false;
			m_Renderer->SetRow(0);
		}
		else
		{
			m_Movement.x = 0;
			m_Movement.y = 0;
			m_Timer += deltaTime;
			return;
		}
	}

	//movement (based on physics)
	if (!m_OnPlatformLeft && m_Movement.x < 0)	m_Movement.x = 0;
	if (!m_OnPlatformRight && m_Movement.x > 0)	m_Movement.x = 0;
	if (!m_OnStairsTop && m_Movement.y < 0)	m_Movement.y = 0;
	if (!m_OnStairsBottom && m_Movement.y > 0)	m_Movement.y = 0;

	glm::vec3 movement{ m_Movement.x * deltaTime * m_Acceleration ,
					   m_Movement.y * deltaTime * m_Acceleration ,
					   0 };

	m_Direction.x += movement.x;
	m_Direction.y += movement.y;
	if (glm::abs(m_Direction.x) > m_Speed * deltaTime)m_Direction.x = m_Speed * deltaTime * glm::sign(m_Direction.x);
	if (glm::abs(m_Direction.y) > m_Speed * deltaTime)m_Direction.y = m_Speed * deltaTime * glm::sign(m_Direction.y);

	GetOwner()->Move(m_Direction);

	//animations
	if (m_Direction.y != 0 || (m_OnStairsBottom && m_OnStairsTop && (!m_OnPlatformLeft || !m_OnPlatformRight)))
	{
		m_Renderer->SetRow(3);
	}
	else if (m_Direction.x < 0)
	{
		m_Renderer->SetRow(1);
	}
	else if (m_Direction.x > 0)
	{
		m_Renderer->SetRow(2);
	}
	else
	{
		m_Renderer->SetRow(0);
	}

	//snap to correct place
	if (m_Movement.x != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(position.x, m_PlatformHeight, position.z));
	}
	if (m_Movement.y != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(m_StairOffset, position.y, position.z));
	}

	//reset
	if (m_Movement.x == 0)
	{
		m_Direction.x -= m_Friction * deltaTime * glm::sign(m_Direction.x);
		if (glm::abs(m_Direction.x) < 1) m_Direction.x = 0;
	}
	if (m_Movement.y == 0)
	{
		m_Direction.y -= m_Friction * deltaTime * glm::sign(m_Direction.y);
		if (glm::abs(m_Direction.y) < 1) m_Direction.y = 0;
	}

	m_Movement.x = 0;
	m_Movement.y = 0;

	m_OnPlatformLeft = false;
	m_OnPlatformRight = false;
	m_OnStairsBottom = false;
	m_OnStairsTop = false;
}
void dae::EnemyPlayerComponent::OnCollision(Collider* other, Collider* mine)
{
	switch (mine->GetTag()[0])
	{
	case 'T':if (other->GetTag() == "STAIR")
	{
		m_OnStairsTop = true;
		m_StairOffset = other->GetOwner()->GetPosition().GetPosition().x;
	}
			break;
	case 'B':if (other->GetTag() == "STAIR")
	{
		m_OnStairsBottom = true;
		m_StairOffset = other->GetOwner()->GetPosition().GetPosition().x;
	}
			break;
	case 'L':if (other->GetTag() == "PLATFORM")
	{
		m_OnPlatformLeft = true;
		m_PlatformHeight = other->GetOwner()->GetPosition().GetPosition().y - 16;
	}
			break;
	case 'R':if (other->GetTag() == "PLATFORM")
	{
		m_OnPlatformRight = true;
		m_PlatformHeight = other->GetOwner()->GetPosition().GetPosition().y - 16;
	}
			break;
	}
}
void dae::EnemyPlayerComponent::OnCollisionEnter(Collider* other, Collider*)
{
	if (other->GetTag() == "PLAYER")
	{
		EventManager::SendEvent("DIE");
	}
	if (other->GetTag() == "PEPPER")
	{
		m_Peppered = true;
		m_Timer = 0;
		m_Renderer->SetRow(4);
		//std::cout << "peppered\n";
	}
}
void dae::EnemyPlayerComponent::Start()
{
	m_Renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
}

//input callbacks
void dae::EnemyPlayerComponent::Left()
{
	m_Movement.x += -1;
}
void dae::EnemyPlayerComponent::Right()
{
	m_Movement.x += 1;
}
void dae::EnemyPlayerComponent::Up()
{
	m_Movement.y += -1;
}
void dae::EnemyPlayerComponent::Down()
{
	m_Movement.y += 1;
}
void dae::EnemyPlayerComponent::Restart()
{
	GetOwner()->SetPosition(m_Startpos);
}