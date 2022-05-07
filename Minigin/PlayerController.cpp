#include "MiniginPCH.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Collider.h"

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
}

void dae::PlayerController::Update(float deltaTime)
{
	if (!m_OnPlatformLeft && m_Movement.x < 0)	m_Movement.x = 0;
	if (!m_OnPlatformRight && m_Movement.x > 0)	m_Movement.x = 0;
	if (!m_OnStairsTop && m_Movement.y < 0)	m_Movement.y = 0;
	if (!m_OnStairsBottom && m_Movement.y > 0)	m_Movement.y = 0;

	glm::vec3 movement{m_Movement.x * deltaTime * m_Speed , 
				       m_Movement.y * deltaTime * m_Speed , 
					   0};

	GetOwner()->Move(movement);

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
	case 'T':if(other->GetTag() == "STAIR") m_OnStairsTop = true;	break;
	case 'B':if(other->GetTag() == "STAIR") m_OnStairsBottom = true;	break;
	case 'L':if(other->GetTag() == "PLATFORM") m_OnPlatformLeft = true;	break;
	case 'R':if(other->GetTag() == "PLATFORM") m_OnPlatformRight = true;	break;
	}
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