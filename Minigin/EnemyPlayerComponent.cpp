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
	m_startpos = GetOwner()->GetPosition().GetPosition();
	//input
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_LEFT", std::bind(&dae::EnemyPlayerComponent::Left, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_RIGHT", std::bind(&dae::EnemyPlayerComponent::Right, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_UP", std::bind(&dae::EnemyPlayerComponent::Up, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_DOWN", std::bind(&dae::EnemyPlayerComponent::Down, this, m_Argument));
	EventManager::AddEvent("DIE", std::bind(&dae::EnemyPlayerComponent::Restart, this, m_Argument));
}
dae::EnemyPlayerComponent::~EnemyPlayerComponent()
{
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_LEFT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_RIGHT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_UP");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_DOWN");
	EventManager::RemoveEvent("PEPPER");

	//SoundManager::GetInstance().RemoveSound("Footstep.wav");
}

void dae::EnemyPlayerComponent::Update(float deltaTime)
{
	//pepper
	if (m_peppered)
	{
		if (m_timer >= m_PepperTime)
		{
			m_peppered = false;
			m_renderer->SetRow(0);
		}
		else
		{
			m_Movement.x = 0;
			m_Movement.y = 0;
			m_timer += deltaTime;
			return;
		}
	}

	//movement (based on physics)
	if (!m_OnPlatformLeft && m_Movement.x < 0)	m_Movement.x = 0;
	if (!m_OnPlatformRight && m_Movement.x > 0)	m_Movement.x = 0;
	if (!m_OnStairsTop && m_Movement.y < 0)	m_Movement.y = 0;
	if (!m_OnStairsBottom && m_Movement.y > 0)	m_Movement.y = 0;

	glm::vec3 movement{ m_Movement.x * deltaTime * m_acceleration ,
					   m_Movement.y * deltaTime * m_acceleration ,
					   0 };

	m_direction.x += movement.x;
	m_direction.y += movement.y;
	if (glm::abs(m_direction.x) > m_Speed * deltaTime)m_direction.x = m_Speed * deltaTime * glm::sign(m_direction.x);
	if (glm::abs(m_direction.y) > m_Speed * deltaTime)m_direction.y = m_Speed * deltaTime * glm::sign(m_direction.y);

	GetOwner()->Move(m_direction);

	//animations
	if (m_direction.y != 0 || (m_OnStairsBottom && m_OnStairsTop && (!m_OnPlatformLeft || !m_OnPlatformRight)))
	{
		m_renderer->SetRow(3);
	}
	else if (m_direction.x < 0)
	{
		m_renderer->SetRow(1);
	}
	else if (m_direction.x > 0)
	{
		m_renderer->SetRow(2);
	}
	else
	{
		m_renderer->SetRow(0);
	}

	//snap to correct place
	if (m_Movement.x != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(position.x, m_platformHeight, position.z));
	}
	if (m_Movement.y != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(m_stairOffset, position.y, position.z));
	}

	//reset
	if (m_Movement.x == 0)
	{
		m_direction.x -= m_Friction * deltaTime * glm::sign(m_direction.x);
		if (glm::abs(m_direction.x) < 1) m_direction.x = 0;
	}
	if (m_Movement.y == 0)
	{
		m_direction.y -= m_Friction * deltaTime * glm::sign(m_direction.y);
		if (glm::abs(m_direction.y) < 1) m_direction.y = 0;
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
		m_stairOffset = other->GetOwner()->GetPosition().GetPosition().x;
	}
			break;
	case 'B':if (other->GetTag() == "STAIR")
	{
		m_OnStairsBottom = true;
		m_stairOffset = other->GetOwner()->GetPosition().GetPosition().x;
	}
			break;
	case 'L':if (other->GetTag() == "PLATFORM")
	{
		m_OnPlatformLeft = true;
		m_platformHeight = other->GetOwner()->GetPosition().GetPosition().y - 16;
	}
			break;
	case 'R':if (other->GetTag() == "PLATFORM")
	{
		m_OnPlatformRight = true;
		m_platformHeight = other->GetOwner()->GetPosition().GetPosition().y - 16;
	}
			break;
	}
}
void dae::EnemyPlayerComponent::OnCollisionEnter(Collider* other, Collider*)
{
	if (other->GetTag() == "PLAYER")
	{
		EventManager::SendEvent("DIE",0);
	}
	if (other->GetTag() == "PEPPER")
	{
		m_peppered = true;
		m_timer = 0;
		m_renderer->SetRow(4);
		//std::cout << "peppered\n";
	}
}
void dae::EnemyPlayerComponent::Start()
{
	m_renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
}

//input callbacks
void dae::EnemyPlayerComponent::Left(byte*)
{
	m_Movement.x += -1;
}
void dae::EnemyPlayerComponent::Right(byte*)
{
	m_Movement.x += 1;
}
void dae::EnemyPlayerComponent::Up(byte*)
{
	m_Movement.y += -1;
}
void dae::EnemyPlayerComponent::Down(byte*)
{
	m_Movement.y += 1;
}
void dae::EnemyPlayerComponent::Restart(byte*)
{
	GetOwner()->SetPosition(m_startpos);
}