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
	EventManager::AddEvent("0BUTTON_A", std::bind(&dae::GameManagerSolo::OnSalt, this));

	m_healthImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Health.png");
	m_pepperImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "pepper.png");

	InitialiseData();
	AddGameUI();
}

dae::GameManagerSolo::~GameManagerSolo()
{
	EventManager::RemoveEvent("DIE");
	EventManager::RemoveEvent("BURGERDONE");
	EventManager::RemoveEvent("0BUTTON_A");
}

void dae::GameManagerSolo::Update(float deltaTime)
{
	m_WasPressingPepper = m_PressingPepper;
	m_PressingPepper = false;

	if (m_scoreToAdd > 0)
	{
		m_score += m_scoreToAdd;
		m_scoreToAdd = 0;
		m_scoreString = std::to_string(m_score);
		UIManager::GetInstance().UpdateUI();
	}

	if (m_endScreen)
	{
		if (m_endScreenTimer >= m_endScreenTime)
		{
			if (m_Lost)
			{
				UIManager::GetInstance().ClearUI();
				SceneManager::GetInstance().SwitchScene("MainMenu.json");
			}
			else
			{
				UIManager::GetInstance().ClearUI();
				if (m_level == 1)
					SceneManager::GetInstance().SwitchScene("Level2.json");
				else
					SceneManager::GetInstance().SwitchScene("MainMenu.json");
			}
		}
		else m_endScreenTimer += deltaTime;
	}
}

void dae::GameManagerSolo::Start()
{
	m_playerObject = SceneManager::GetInstance().FindComponent<dae::PlayerController>()->GetOwner();
	m_playerStartpos = m_playerObject->GetPosition().GetPosition();

	m_enemySpawner = static_cast<EnemySpawner*>(SceneManager::GetInstance().FindComponent<EnemySpawner>());
}

void dae::GameManagerSolo::Restart()
{
	m_healthString = std::to_string(m_health);
	m_playerObject->SetPosition(m_playerStartpos);
	if(m_enemySpawner != nullptr) m_enemySpawner->Restart();

	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManagerSolo::GameOver()
{
	m_Lost = true;
	m_endScreen = true;

	HandleScore();
	UIManager::GetInstance().AddTextElement(&m_YouLost,64,glm::vec2(70,210),SDL_Color(0,0,0));
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(60, 200), SDL_Color(210, 50, 50));
	UIManager::GetInstance().UpdateUI();

	m_enemySpawner->SetSpawning(false);
	m_enemySpawner->ClearEnemys();

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
	UIManager::GetInstance().AddImageElement(&m_pepperImage, glm::vec2(32, 32), glm::vec2(365, 15));
	UIManager::GetInstance().AddTextElement(&m_pepperString, m_FontSize - 10, glm::vec2(395, 20));

	//Health
	UIManager::GetInstance().AddImageElement(&m_healthImage, glm::vec2(32, 32), glm::vec2(425, 12));
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
		m_appDataPath = szPath;
	}
	CreateDirectory(m_appDataPath.c_str(), NULL);

	//create read me for future ref
	std::ofstream out{m_appDataPath + "ReadMe.txt"};
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
	if (m_score > m_HighScore)
	{
		m_HighScore = m_score;
	}
	std::ofstream out{m_SoloDataPath.c_str()};
	out << m_HighScore;
	out.close();
}

void dae::GameManagerSolo::OnDie()
{
	m_daed = true;
	m_health--;
	if (m_health == 0)
		GameOver();
	else if (m_health > 0)
		Restart();
}

void dae::GameManagerSolo::OnWin()
{
	m_Lost = false;
	m_endScreen = true;
	m_enemySpawner->SetSpawning(false);
	m_enemySpawner->ClearEnemys();

	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(60, 200), SDL_Color(50, 210, 50));
	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("NextLevel.wav");
	EventManager::RemoveEvent("0BUTTON_A");
	HandleScore();
}

void dae::GameManagerSolo::OnSalt()
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

void dae::GameManagerSolo::OnBurgderDone()
{
	m_doneBurgers++;
	if (m_doneBurgers == 16)
	{
		OnWin();
	}
}

void dae::GameManagerSolo::AddScore(int score)
{
	m_scoreToAdd += score;
}
