#include "MiniginPCH.h"
#include "MainMenu.h"
#include "UIManager.h"
#include "ResourceManager.h"
#include "Font.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneManager.h"

dae::MainMenu::MainMenu(GameObject* owner) : Component(owner)
{
	//UI
	m_Logo = ResourceManager::GetInstance().LoadTexture(basePath + "Logo.png");
	UIManager::GetInstance().AddImageElement(&m_Logo, glm::vec2(320,148), glm::vec2(85, 50));

	m_Arrow = ResourceManager::GetInstance().LoadTexture(basePath + "Arrow.png");
	int i1 = UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 250));
	int i2 = UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 300));

	m_Arrow1 = UIManager::GetInstance().getElement(i1);
	m_Arrow2 = UIManager::GetInstance().getElement(i2);
	m_Arrow2->SetActive(false);

	UIManager::GetInstance().AddTextElement(&m_Player1,35,glm::vec2(190,250));
	UIManager::GetInstance().AddTextElement(&m_Player2,35,glm::vec2(183,300));

	//Input
	EventManager::AddEvent("0BUTTON_A",std::bind(&dae::MainMenu::OnA,this,m_Argument));
	EventManager::AddEvent("0BUTTON_UP",std::bind(&dae::MainMenu::OnUp,this,m_Argument));
	EventManager::AddEvent("0BUTTON_DOWN",std::bind(&dae::MainMenu::OnDown,this,m_Argument));

	UIManager::GetInstance().UpdateUI();
}

dae::MainMenu::~MainMenu()
{
	//EventManager::ClearEvents();
	EventManager::RemoveEvent("0BUTTON_A");
	EventManager::RemoveEvent("0BUTTON_UP");
	EventManager::RemoveEvent("0BUTTON_DOWN");
}

void dae::MainMenu::OnDown(byte*)
{
	m_firstPos = false;
	m_Arrow1->SetActive(false);
	m_Arrow2->SetActive(true);
}
void dae::MainMenu::OnUp(byte*)
{
	m_firstPos = true;
	m_Arrow1->SetActive(true);
	m_Arrow2->SetActive(false);
}
void dae::MainMenu::OnA(byte*)
{
	UIManager::GetInstance().ClearUI();
	if (m_firstPos)
	{
		SceneManager::GetInstance().SwitchScene("Level1.json");
	}
	else
	{
		SceneManager::GetInstance().SwitchScene("2Level1.json");
	}
}