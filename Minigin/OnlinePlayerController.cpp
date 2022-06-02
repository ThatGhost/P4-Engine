#include "MiniginPCH.h"
#include "OnlinePlayerController.h"
#include "SceneManager.h"
#include "GameManagerOnline.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Collider.h"
#include "RenderComponent.h"
#include "DestroyComponent.h"
#include "SoundManager.h"


dae::OnlinePlayerController::OnlinePlayerController(GameObject* owner) : Component(owner)
{
	GameManagerOnline* o = static_cast<GameManagerOnline*>(SceneManager::GetInstance().FindComponent<GameManagerOnline>());
	o->SetPlayer(this);
	SoundManager::GetInstance().PlaySound(m_WalkingSound, true, 1);
}

dae::OnlinePlayerController::~OnlinePlayerController()
{
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_LEFT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_RIGHT");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_UP");
	EventManager::RemoveEvent(std::to_string(m_Player) + "BUTTON_DOWN");
	EventManager::RemoveEvent(std::to_string(m_Player) + "PEPPER");

	SoundManager::GetInstance().RemoveSound("Footstep.wav");
}

void dae::OnlinePlayerController::Update(float deltaTime)
{
	//movement
	if (!m_OnPlatformLeft && m_Movement.x < 0)	m_Movement.x = 0;
	if (!m_OnPlatformRight && m_Movement.x > 0)	m_Movement.x = 0;
	if (!m_OnStairsTop && m_Movement.y < 0)	m_Movement.y = 0;
	if (!m_OnStairsBottom && m_Movement.y > 0)	m_Movement.y = 0;

	glm::vec3 movement{ m_Movement.x * deltaTime * m_Speed ,
					   m_Movement.y * deltaTime * m_Speed ,
					   0 };

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
		SoundManager::GetInstance().PauseSound(false, 1);
	}
	else
	{
		SoundManager::GetInstance().PauseSound(true, 1);
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
	m_Movement.x = 0;
	m_Movement.y = 0;

	m_OnPlatformLeft = false;
	m_OnPlatformRight = false;
	m_OnStairsBottom = false;
	m_OnStairsTop = false;
}
void dae::OnlinePlayerController::OnCollision(Collider* other, Collider* mine)
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
	if (mine->GetTag() == "ENEMYPLAYER" && other->GetTag() == "PLAYER")
	{
		EventManager::SendEvent("GAMEOVER");
	}
}
void dae::OnlinePlayerController::Init(int player)
{
	m_Player = player;
	m_renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_LEFT", std::bind(&dae::OnlinePlayerController::Left, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_RIGHT", std::bind(&dae::OnlinePlayerController::Right, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_UP", std::bind(&dae::OnlinePlayerController::Up, this));
	EventManager::AddEvent(std::to_string(m_Player) + "BUTTON_DOWN", std::bind(&dae::OnlinePlayerController::Down, this));
	EventManager::AddEvent(std::to_string(m_Player) + "PEPPER", std::bind(&dae::OnlinePlayerController::Pepper, this));
}

void dae::OnlinePlayerController::SetGood(bool good)
{
	m_Good = good;
	if (m_Good)
	{
		//white
		//interact with burgers
		Collider* coll = static_cast<Collider*>(GetOwner()->AddComponent<Collider>());
		coll->SetPosition(glm::vec3(7,3,0));
		coll->SetDimensions(glm::vec2(18,27));
		coll->Setlookers(true, false);
		coll->SetTag("PLAYER");

		m_renderer->SetImage("Player.png",true,1);
	}
	else
	{
		//red
		//interact with player
		Collider* coll = static_cast<Collider*>(GetOwner()->AddComponent<Collider>());
		coll->SetPosition(glm::vec3(7, 3, 0));
		coll->SetDimensions(glm::vec2(18, 27));
		coll->Setlookers(true, false);
		coll->SetTag("ENEMYPLAYER");

		m_renderer->SetImage("EvilCook.png", true, 1);
	}
}

//input callbacks
void dae::OnlinePlayerController::Left()
{
	m_Movement.x += -1;
}
void dae::OnlinePlayerController::Right()
{
	m_Movement.x += 1;
}
void dae::OnlinePlayerController::Up()
{
	m_Movement.y += -1;
}
void dae::OnlinePlayerController::Down()
{
	m_Movement.y += 1;
}
void dae::OnlinePlayerController::Pepper()
{
	GameObject* go = new GameObject();
	go->SetParent(GetOwner());
	go->SetStatic(false);

	glm::vec3 myPos{ GetOwner()->GetPosition().GetPosition() };
	float pepperpos{ m_Movement.x * 28 };
	glm::vec3 pos{ pepperpos, 0, 2 };
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