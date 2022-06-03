#include "MiniginPCH.h"
#include "GameManagerSolo.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "PlayerController.h"
#include "EnemySpawner.h"
#include "SoundManager.h"

#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <fstream>

dae::GameManagerSolo::GameManagerSolo(GameObject* owner) : GameManager(owner)
{
	EventManager::AddEvent("DIE",std::bind(&dae::GameManagerSolo::OnDie,this));
	EventManager::AddEvent("BURGERDONE",std::bind(&dae::GameManagerSolo::OnBurgderDone,this));
	EventManager::AddEvent("0BUTTON_X", std::bind(&dae::GameManagerSolo::OnPepper, this));

	m_HealthImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Health.png");
	m_PepperImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "pepper.png");

	InitialiseData();
	AddGameUI();
}

dae::GameManagerSolo::~GameManagerSolo()
{
	EventManager::RemoveEvent("DIE");
	EventManager::RemoveEvent("BURGERDONE");
	EventManager::RemoveEvent("0BUTTON_X");
}

void dae::GameManagerSolo::Update(float deltaTime)
{
	m_WasPressingPepper = m_PressingPepper;
	m_PressingPepper = false;

	if (m_ScoreToAdd > 0)
	{
		m_Score += m_ScoreToAdd;
		m_ScoreToAdd = 0;
		m_scoreString = std::to_string(m_Score);
		UIManager::GetInstance().UpdateUI();
	}

	if (m_EndScreen)
	{
		if (m_EndScreenTimer >= m_EndScreenTime)
		{
			if (m_Lost)
			{
				UIManager::GetInstance().ClearUI();
				SceneManager::GetInstance().SwitchScene("MainMenu.json");
			}
			else
			{
				UIManager::GetInstance().ClearUI();
				if (m_Level == 1)
					SceneManager::GetInstance().SwitchScene("Level2.json");
				else
					SceneManager::GetInstance().SwitchScene("MainMenu.json");
			}
		}
		else m_EndScreenTimer += deltaTime;
	}
}

void dae::GameManagerSolo::Start()
{
	m_PlayerObject = SceneManager::GetInstance().FindComponent<dae::PlayerController>()->GetOwner();
	m_PlayerStartpos = m_PlayerObject->GetPosition().GetPosition();

	m_EnemySpawner = static_cast<EnemySpawner*>(SceneManager::GetInstance().FindComponent<EnemySpawner>());
}

void dae::GameManagerSolo::Restart()
{
	m_healthString = std::to_string(m_Health);
	m_PlayerObject->SetPosition(m_PlayerStartpos);
	if(m_EnemySpawner != nullptr) m_EnemySpawner->Restart();

	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManagerSolo::GameOver()
{
	m_Lost = true;
	m_EndScreen = true;

	HandleScore();
	UIManager::GetInstance().AddTextElement(&m_YouLost,64,glm::vec2(70,210),SDL_Color(0,0,0));
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(60, 200), SDL_Color(210, 50, 50));
	UIManager::GetInstance().UpdateUI();

	m_EnemySpawner->SetSpawning(false);
	m_EnemySpawner->ClearEnemys();

	EventManager::RemoveEvent("0BUTTON_A");
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManagerSolo::AddGameUI()
{
	//score
	UIManager::GetInstance().AddTextElement(&m_1Up, m_FontSize, glm::vec2(20, 10),SDL_Color(0,255,0));
	UIManager::GetInstance().AddTextElement(&m_scoreString, m_FontSize - 10, glm::vec2(90, 20));

	//Hi score
	UIManager::GetInstance().AddTextElement(&m_Hi_Score, m_FontSize, glm::vec2(210, 10), SDL_Color(255, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_highScoreString, m_FontSize - 10, glm::vec2(250, 20));

	//pepper
	UIManager::GetInstance().AddImageElement(&m_PepperImage, glm::vec2(32, 32), glm::vec2(365, 15));
	UIManager::GetInstance().AddTextElement(&m_pepperString, m_FontSize - 10, glm::vec2(395, 20));

	//Health
	UIManager::GetInstance().AddImageElement(&m_HealthImage, glm::vec2(32, 32), glm::vec2(425, 12));
	UIManager::GetInstance().AddTextElement(&m_healthString, m_FontSize - 10, glm::vec2(455, 20));

	UIManager::GetInstance().UpdateUI();
}

void dae::GameManagerSolo::InitialiseData()
{
	//create main directory
	TCHAR szPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath)))
	{
		PathAppend(szPath, _T("\\.STUDENT_PROJECT_DATA\\"));
		m_AppDataPath = szPath;
	}
	CreateDirectory(m_AppDataPath.c_str(), NULL);

	//create read me for future ref
	std::ofstream out{m_AppDataPath + "ReadMe.txt"};
	out << m_ReadMeText;
	out.close();

	//create data folder path
	m_SoloDataPath = szPath;
	m_SoloDataPath += "\\SOLO\\";
	CreateDirectory(m_SoloDataPath.c_str(), NULL);
	m_SoloDataPath += "ScoreData.dat";

	//make score file if it doesn't exsist
	std::ifstream in{m_SoloDataPath};
	if (!in.good())
	{
		std::ofstream o{ m_SoloDataPath };
		o << 0;
		o.close();
		in = std::ifstream{ m_SoloDataPath };
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

void dae::GameManagerSolo::HandleScore() //The most basic type of saving lol
{
	if (m_Score > m_HighScore)
	{
		m_HighScore = m_Score;
	}
	std::ofstream out{m_SoloDataPath.c_str()};
	out << m_HighScore;
	out.close();
}

void dae::GameManagerSolo::OnDie()
{
	m_Dead = true;
	m_Health--;
	if (m_Health == 0)
		GameOver();
	else if (m_Health > 0)
		Restart();
}

void dae::GameManagerSolo::OnWin()
{
	m_Lost = false;
	m_EndScreen = true;
	m_EnemySpawner->SetSpawning(false);
	m_EnemySpawner->ClearEnemys();

	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(60, 200), SDL_Color(50, 210, 50));
	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("NextLevel.wav");
	EventManager::RemoveEvent("0BUTTON_A");
	HandleScore();
}

void dae::GameManagerSolo::OnPepper()
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

void dae::GameManagerSolo::OnBurgderDone()
{
	m_DoneBurgers++;
	if (m_DoneBurgers == 16)
	{
		OnWin();
	}
}

void dae::GameManagerSolo::AddScore(int score)
{
	m_ScoreToAdd += score;
}