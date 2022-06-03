#include "MiniginPCH.h"
#include "GameManagerVersus.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "PlayerController.h"
#include "SoundManager.h"

dae::GameManagerVersus::GameManagerVersus(GameObject* owner) : GameManager(owner)
{
	EventManager::AddEvent("DIE", std::bind(&dae::GameManagerVersus::OnDie, this));
	EventManager::AddEvent("BURGERDONE", std::bind(&dae::GameManagerVersus::OnBurgderDone, this));
	EventManager::AddEvent("0BUTTON_X", std::bind(&dae::GameManagerVersus::OnPepper, this));

	m_HealthImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Health.png");
	m_PepperImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "pepper.png");

	AddGameUI();
}

dae::GameManagerVersus::~GameManagerVersus()
{
	EventManager::RemoveEvent("DIE");
	EventManager::RemoveEvent("BURGERDONE");
	EventManager::RemoveEvent("0BUTTON_X");
}

void dae::GameManagerVersus::Update(float)
{
	m_WasPressingPepper = m_PressingPepper;
	m_PressingPepper = false;
}

void dae::GameManagerVersus::Start()
{
	m_PlayerController = SceneManager::GetInstance().FindComponent<dae::PlayerController>()->GetOwner();
	m_PlayerStartpos = m_PlayerController->GetPosition().GetPosition();
}

void dae::GameManagerVersus::Restart()
{
	//remove all enemies and place player in start
	m_healthString = std::to_string(m_Health);
	m_PlayerController->SetPosition(m_PlayerStartpos);
	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManagerVersus::GameOver()
{
	UIManager::GetInstance().ClearUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManagerVersus::AddGameUI()
{
	//pepper
	UIManager::GetInstance().AddImageElement(&m_PepperImage, glm::vec2(32, 32), glm::vec2(365, 15));
	UIManager::GetInstance().AddTextElement(&m_pepperString, m_FontSize - 10, glm::vec2(395, 20));

	//Health
	UIManager::GetInstance().AddImageElement(&m_HealthImage, glm::vec2(32, 32), glm::vec2(425, 12));
	UIManager::GetInstance().AddTextElement(&m_healthString, m_FontSize - 10, glm::vec2(455, 20));

	UIManager::GetInstance().UpdateUI();
}

void dae::GameManagerVersus::OnDie()
{
	m_Health--;
	if (m_Health == 0)
		GameOver();
	else if (m_Health > 0)
		Restart();
}

void dae::GameManagerVersus::OnWin()
{
	SoundManager::GetInstance().PlaySound("NextLevel.wav");
	UIManager::GetInstance().ClearUI();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManagerVersus::OnPepper()
{
	if (!m_WasPressingPepper && m_Pepper > 0)
	{
		m_Pepper--;
		m_pepperString = std::to_string(m_Pepper);
		EventManager::SendEvent("0PEPPER");
		UIManager::GetInstance().UpdateUI();
	}
	m_PressingPepper = true;
}

void dae::GameManagerVersus::OnBurgderDone()
{
	m_DoneBurgers++;
	if (m_DoneBurgers == 16)
	{
		OnWin();
	}
}