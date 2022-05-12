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
	m_arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 250)));
	m_arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 300)));
	m_arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 350)));
	m_arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 400)));

	DisableAllArrows();
	m_arrows[0]->SetActive(true);

	UIManager::GetInstance().AddTextElement(&m_Player1,32,glm::vec2(160,250));
	UIManager::GetInstance().AddTextElement(&m_Player2,32,glm::vec2(150,300));
	UIManager::GetInstance().AddTextElement(&m_Coop,32,glm::vec2(213,350));
	UIManager::GetInstance().AddTextElement(&m_Online,32,glm::vec2(213,400));

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

void dae::MainMenu::Update(float)
{
	m_wasPressed = m_isPressing;
	m_isPressing = false;
}

void dae::MainMenu::handleInput()
{
	if (!m_wasPressed)
	{
		if (m_isDown)
		{
			m_menuposition = MenuPosition((int)m_menuposition + 1);
			if (m_menuposition == MenuPosition::end)
				m_menuposition = MenuPosition::solo;
		}
		else
		{
			m_menuposition = MenuPosition((int)m_menuposition - 1);
			if (int(m_menuposition) == -1)
			{
				m_menuposition = MenuPosition::online;
			}
		}
		DisableAllArrows();
		m_arrows[(int)m_menuposition]->SetActive(true);
	}
}

void dae::MainMenu::DisableAllArrows()
{
	for (auto arrow : m_arrows)
	{
		arrow->SetActive(false);
	}
}

void dae::MainMenu::OnDown(byte*)
{
	m_isPressing = true;
	m_isDown = true;
	handleInput();
}

void dae::MainMenu::OnUp(byte*)
{
	m_isPressing = true;
	m_isDown = false;
	handleInput();
}

void dae::MainMenu::OnA(byte*)
{
	UIManager::GetInstance().ClearUI();
	switch (m_menuposition)
	{
	case MenuPosition::solo:
		InputManager::GetInstance().SetKeyboardId(0);
		SceneManager::GetInstance().SwitchScene("Level1.json");
		break;
	case MenuPosition::versus:
		InputManager::GetInstance().SetKeyboardId(1);
		SceneManager::GetInstance().SwitchScene("Versus1.json");
		break;
	case MenuPosition::coop:
		InputManager::GetInstance().SetKeyboardId(1);
		SceneManager::GetInstance().SwitchScene("Coop1.json");
		break;
	case MenuPosition::online:
		InputManager::GetInstance().SetKeyboardId(0);
		SceneManager::GetInstance().SwitchScene("Versus1.json");
		break;
	}
}