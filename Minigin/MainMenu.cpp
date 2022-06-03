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
	m_Logo = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Logo.png");
	UIManager::GetInstance().AddImageElement(&m_Logo, glm::vec2(320,148), glm::vec2(85, 50));

	m_Arrow = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Arrow.png");
	m_Arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 250)));
	m_Arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 300)));
	m_Arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 350)));
	m_Arrows.push_back(UIManager::GetInstance().AddImageElement(&m_Arrow, glm::vec2(32, 32), glm::vec2(120, 400)));

	DisableAllArrows();
	m_Arrows[0]->SetActive(true);

	UIManager::GetInstance().AddTextElement(&m_Player1,32,glm::vec2(160,250));
	UIManager::GetInstance().AddTextElement(&m_Player2,32,glm::vec2(150,300));
	UIManager::GetInstance().AddTextElement(&m_Coop,32,glm::vec2(213,350));
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
	UIManager::GetInstance().AddTextElement(&m_Online, 32, glm::vec2(213, 400));
#else
#define ENVIRONMENT32
	UIManager::GetInstance().AddTextElement(&m_Online, 32, glm::vec2(213, 400),SDL_Color(130,130,130));
#endif
#endif

	//Input
	EventManager::AddEvent("0BUTTON_A",std::bind(&dae::MainMenu::OnA,this));
	EventManager::AddEvent("0BUTTON_UP",std::bind(&dae::MainMenu::OnUp,this));
	EventManager::AddEvent("0BUTTON_DOWN",std::bind(&dae::MainMenu::OnDown,this));

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
	m_WasPressed = m_IsPressing;
	m_IsPressing = false;
}

void dae::MainMenu::handleInput()
{
	if (!m_WasPressed)
	{
		if (m_IsDown)
		{
			m_MenuPosition = MenuPosition((int)m_MenuPosition + 1);
			if (m_MenuPosition == MenuPosition::end)
				m_MenuPosition = MenuPosition::solo;
		}
		else
		{
			m_MenuPosition = MenuPosition((int)m_MenuPosition - 1);
			if (int(m_MenuPosition) == -1)
			{
				m_MenuPosition = MenuPosition::online;
			}
		}
		DisableAllArrows();
		m_Arrows[(int)m_MenuPosition]->SetActive(true);
	}
}

void dae::MainMenu::DisableAllArrows()
{
	for (auto arrow : m_Arrows)
	{
		arrow->SetActive(false);
	}
}

void dae::MainMenu::OnDown()
{
	m_IsPressing = true;
	m_IsDown = true;
	handleInput();
}

void dae::MainMenu::OnUp()
{
	m_IsPressing = true;
	m_IsDown = false;
	handleInput();
}

void dae::MainMenu::OnA()
{
	switch (m_MenuPosition)
	{
	case MenuPosition::solo:
		UIManager::GetInstance().ClearUI();
		InputManager::GetInstance().SetKeyboardId(0);
		SceneManager::GetInstance().SwitchScene("Level1.json");
		break;
	case MenuPosition::versus:
		UIManager::GetInstance().ClearUI();
		InputManager::GetInstance().SetKeyboardId(1);
		SceneManager::GetInstance().SwitchScene("Versus1.json");
		break;
	case MenuPosition::coop:
		UIManager::GetInstance().ClearUI();
		InputManager::GetInstance().SetKeyboardId(1);
		SceneManager::GetInstance().SwitchScene("Coop1.json");
		break;
#if _WIN64
#define ENVIROMENT64
	case MenuPosition::online:
		UIManager::GetInstance().ClearUI();
		InputManager::GetInstance().SetKeyboardId(0);
		SceneManager::GetInstance().SwitchScene("ServerBrowser.json");
#endif
		break;
	}
}