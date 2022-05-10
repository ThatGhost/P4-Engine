#include "MiniginPCH.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Collider.h"
#include "RenderComponent.h"
#include "DestroyComponent.h"
#include "SceneManager.h"
#include "SoundManager.h"

//TEMP
#include "FpsComponent.h"

dae::PlayerController::PlayerController(GameObject* owner) : Component(owner)
{
	//TEMP
	//GetOwner()->AddComponent<dae::FpsComponent>();

	//input
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_LEFT", std::bind(&dae::PlayerController::Left, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_RIGHT", std::bind(&dae::PlayerController::Right, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_UP", std::bind(&dae::PlayerController::Up, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_DOWN", std::bind(&dae::PlayerController::Down, this, m_Argument));
	EventManager::AddEvent("PEPPER", std::bind(&dae::PlayerController::Pepper, this, m_Argument));
	SoundManager::GetInstance().PlaySound(m_WalkingSound, true);
}
dae::PlayerController::~PlayerController()
{
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_LEFT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_RIGHT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_UP");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_DOWN");
	EventManager::RemoveEvent("PEPPER");

	SoundManager::GetInstance().RemoveSound("Footstep.wav");
}

void dae::PlayerController::Update(float deltaTime)
{
	//movement
	if (!m_OnPlatformLeft && m_Movement.x < 0)	m_Movement.x = 0;
	if (!m_OnPlatformRight && m_Movement.x > 0)	m_Movement.x = 0;
	if (!m_OnStairsTop && m_Movement.y < 0)	m_Movement.y = 0;
	if (!m_OnStairsBottom && m_Movement.y > 0)	m_Movement.y = 0;

	glm::vec3 movement{m_Movement.x * deltaTime * m_Speed , 
				       m_Movement.y * deltaTime * m_Speed , 
					   0};

	GetOwner()->Move(movement);

	//animations
	if (m_Movement.y != 0 || (m_OnStairsBottom && m_OnStairsTop && (!m_OnPlatformLeft || !m_OnPlatformRight)))
	{
		m_renderer->SetRow(3);
	}
	else if (m_Movement.x < 0)
	{
		m_renderer->SetRow(1);
	}
	else if (m_Movement.x > 0)
	{
		m_renderer->SetRow(2);
	}
	else
	{
		m_renderer->SetRow(0);
	}

	//sound
	if (m_Movement.x != 0 || m_Movement.y != 0)
	{
		SoundManager::GetInstance().PauseSound(m_WalkingSound, false);
	}
	else
	{
		SoundManager::GetInstance().PauseSound(m_WalkingSound, true);
	}

	//snap to correct place
	if (m_Movement.x != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(position.x,m_platformHeight,position.z));
	}
	if (m_Movement.y != 0)
	{
		glm::vec3 position = GetOwner()->GetPosition().GetPosition();
		GetOwner()->SetPosition(glm::vec3(m_stairOffset,position.y,position.z));
	}

	//reset
	m_Movement.x = 0;
	m_Movement.y = 0;

	m_OnPlatformLeft = false;
	m_OnPlatformRight = false;
	m_OnStairsBottom = false;
	m_OnStairsTop = false;
}
void dae::PlayerController::OnCollision(Collider* other, Collider* mine)
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
void dae::PlayerController::Init(int player)
{
	m_Player = player;
	m_renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
}

//input callbacks
void dae::PlayerController::Left(byte*)
{
	m_Movement.x += -1;
}
void dae::PlayerController::Right(byte*)
{
	m_Movement.x += 1;
}
void dae::PlayerController::Up(byte*)
{
	m_Movement.y += -1;
}
void dae::PlayerController::Down(byte*)
{
	m_Movement.y += 1;
}
void dae::PlayerController::Pepper(byte*)
{
	GameObject* go = new GameObject();
	go->SetParent(GetOwner());
	go->SetStatic(false);

	glm::vec3 myPos{GetOwner()->GetPosition().GetPosition()};
	float pepperpos{ m_Movement.x * 28 };
	glm::vec3 pos{pepperpos, 0, 2};
	go->SetPosition(pos);

	Collider* coll = static_cast<Collider*>(go->AddComponent<Collider>());
	coll->Setlookers(true, false);
	coll->SetPosition(glm::vec3(0, 0, 0));
	coll->SetDimensions(glm::vec2(20, 20));
	coll->SetTag("PEPPER");
	SceneManager::GetInstance().AddCollider(coll);

	DestroyComponent* destroy = static_cast<DestroyComponent*>(go->AddComponent<DestroyComponent>());
	destroy->Init(0.5f);

	RenderComponent* renderer = static_cast<RenderComponent*>(go->AddComponent<RenderComponent>());
	renderer->SetImage("pepper.png");

	//sound
	SoundManager::GetInstance().PlaySound("Shoot.wav");
}