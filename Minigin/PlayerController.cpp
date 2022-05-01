#include "MiniginPCH.h"
#include "PlayerController.h"
#include "InputManager.h"
#include "EventManager.h"

//TEMP
#include "FpsComponent.h"

dae::PlayerController::PlayerController(GameObject* owner) : Component(owner)
{
	//TEMP
	//GetOwner()->AddComponent<dae::FpsComponent>();

	//input
	EventManager::AddEvent(std::to_string(m_Player) + "LEFTBUTTON", std::bind(&dae::PlayerController::Left, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "RIGHTBUTTON", std::bind(&dae::PlayerController::Right, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "UPBUTTON", std::bind(&dae::PlayerController::Up, this, m_Argument));
	EventManager::AddEvent(std::to_string(m_Player) + "DOWNBUTTON", std::bind(&dae::PlayerController::Down, this, m_Argument));

	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonLeft, "LEFTBUTTON");
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonRight, "RIGHTBUTTON");
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonUp, "UPBUTTON");
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonDown, "DOWNBUTTON");

	//collisions
	EventManager::AddEvent("PLATFORML", std::bind(&dae::PlayerController::OnCollisionPlatformL, this, m_Argument));
	EventManager::AddEvent("PLATFORMR", std::bind(&dae::PlayerController::OnCollisionPlatformR, this, m_Argument));
	EventManager::AddEvent("STAIRT", std::bind(&dae::PlayerController::OnCollisionStairsT, this, m_Argument));
	EventManager::AddEvent("STAIRB", std::bind(&dae::PlayerController::OnCollisionStairsB, this, m_Argument));
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

//callbacks
void dae::PlayerController::Left(float)
{
	m_Movement.x += -1;
}
void dae::PlayerController::Right(float)
{
	m_Movement.x += 1;
}
void dae::PlayerController::Up(float)
{
	m_Movement.y += -1;
}
void dae::PlayerController::Down(float)
{
	m_Movement.y += 1;
}

void dae::PlayerController::OnCollisionPlatformL(float)
{
	m_OnPlatformLeft = true;
}
void dae::PlayerController::OnCollisionPlatformR(float)
{
	m_OnPlatformRight = true;
}
void dae::PlayerController::OnCollisionStairsT(float)
{
	m_OnStairsTop = true;
}
void dae::PlayerController::OnCollisionStairsB(float)
{
	m_OnStairsBottom = true;
}