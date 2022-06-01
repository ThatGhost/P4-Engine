#include "MiniginPCH.h"
#include "GameManagerCoop.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "SoundManager.h"

//IO
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <fstream>

dae::GameManagerCoop::GameManagerCoop(GameObject* owner) : GameManager(owner)
{
	EventManager::AddEvent("DIE", std::bind(&dae::GameManagerCoop::OnDie, this));
	EventManager::AddEvent("BURGERDONE", std::bind(&dae::GameManagerCoop::OnBurgderDone, this));
	EventManager::AddEvent("0BUTTON_A", std::bind(&dae::GameManagerCoop::OnSalt, this));
	EventManager::AddEvent("1BUTTON_A", std::bind(&dae::GameManagerCoop::OnSalt2, this));

	m_healthImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Health.png");
	m_pepperImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "pepper.png");

	InitialiseData();
	AddGameUI();
}

dae::GameManagerCoop::~GameManagerCoop()
{
	EventManager::RemoveEvent("DIE");
	EventManager::RemoveEvent("BURGERDONE");
	EventManager::RemoveEvent("0BUTTON_A");
}

void dae::GameManagerCoop::Update(float)
{
	m_WasPressingPepper = m_PressingPepper;
	m_PressingPepper = false;
}

void dae::GameManagerCoop::Start()
{
	m_playerObject = SceneManager::GetInstance().FindComponent<dae::PlayerController>()->GetOwner();
	m_playerStartpos = m_playerObject->GetPosition().GetPosition();

	m_enemySpawner = static_cast<EnemySpawner*>(SceneManager::GetInstance().FindComponent<EnemySpawner>());
}

void dae::GameManagerCoop::Restart()
{
	m_healthString = std::to_string(m_health);
	m_playerObject->SetPosition(m_playerStartpos);
	if (m_enemySpawner != nullptr) m_enemySpawner->Restart();
	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManagerCoop::GameOver()
{
	HandleScore();
	EventManager::RemoveEvent("0BUTTON_A");
	UIManager::GetInstance().ClearUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManagerCoop::AddGameUI()
{
	//score
	UIManager::GetInstance().AddTextElement(&m_1Up, m_FontSize, glm::vec2(20, 10), SDL_Color(0, 255, 0));
	UIManager::GetInstance().AddTextElement(&m_scoreString, m_FontSize - 10, glm::vec2(90, 20));

	//Hi score
	UIManager::GetInstance().AddTextElement(&m_Hi_Score, m_FontSize, glm::vec2(210, 10), SDL_Color(255, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_highScoreString, m_FontSize - 10, glm::vec2(250, 20));

	//pepper
	UIManager::GetInstance().AddImageElement(&m_pepperImage, glm::vec2(32, 32), glm::vec2(365, 15));
	UIManager::GetInstance().AddTextElement(&m_pepperString, m_FontSize - 10, glm::vec2(395, 20));

	//Health
	UIManager::GetInstance().AddImageElement(&m_healthImage, glm::vec2(32, 32), glm::vec2(425, 12));
	UIManager::GetInstance().AddTextElement(&m_healthString, m_FontSize - 10, glm::vec2(455, 20));

	UIManager::GetInstance().UpdateUI();
}

void dae::GameManagerCoop::InitialiseData()
{
	//create main directory
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		PathAppend(szPath, _T("\\.STUDENT_PROJECT_DATA\\"));
		m_appDataPath = szPath;
	}
	CreateDirectory(m_appDataPath.c_str(), NULL);

	//create data folder path
	m_CoopDataPath = szPath;
	m_CoopDataPath += "\\SOLO\\";
	CreateDirectory(m_CoopDataPath.c_str(), NULL);
	m_CoopDataPath += "ScoreData.dat";

	//make score file if it doesn't exsist
	std::ifstream in{ m_CoopDataPath };
	if (!in.good())
	{
		std::ofstream o{ m_CoopDataPath };
		o << 0;
		o.close();
		in = std::ifstream{ m_CoopDataPath };
	}

	//read highscore
	if (in.is_open())
	{
		std::string score{};
		while (std::getline(in, score))
		{
			m_HighScore = std::stoi(score);
			m_highScoreString = std::to_string(m_HighScore);
		}
	}

	in.close();
}

void dae::GameManagerCoop::HandleScore() //The most basic type of saving lol
{
	if (m_score > m_HighScore)
	{
		m_HighScore = m_score;
	}
	std::ofstream out{ m_CoopDataPath.c_str() };
	out << m_HighScore;
	out.close();
}

void dae::GameManagerCoop::OnDie()
{
	m_daed = true;
	m_health--;
	if (m_health == 0)
		GameOver();
	else if (m_health > 0)
		Restart();
}

void dae::GameManagerCoop::OnWin()
{
	SoundManager::GetInstance().PlaySound("NextLevel.wav");
	EventManager::RemoveEvent("0BUTTON_A");
	UIManager::GetInstance().ClearUI();
	HandleScore();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManagerCoop::OnSalt()
{
	if (!m_WasPressingPepper && m_pepper > 0)
	{
		m_pepper--;
		m_pepperString = std::to_string(m_pepper);
		EventManager::SendEvent("0PEPPER");
		UIManager::GetInstance().UpdateUI();
	}
	m_PressingPepper = true;
}

void dae::GameManagerCoop::OnBurgderDone()
{
	m_doneBurgers++;
	if (m_doneBurgers == 16)
	{
		OnWin();
	}
}

void dae::GameManagerCoop::AddScore(int score)
{
	m_score += score;
	m_scoreString = std::to_string(m_score);
	UIManager::GetInstance().UpdateUI();
}
