#include "MiniginPCH.h"
#include "GameManager.h"
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

using byte = unsigned char;
byte* g_arg;

dae::GameManager* dae::GameManager::GetInstance(bool restart)
{
	static GameManager* instance = nullptr;
	if (instance == nullptr || restart)
		instance = static_cast<GameManager*>(SceneManager::GetInstance().FindComponent<GameManager>());

	return instance;
}

dae::GameManager::GameManager(GameObject* owner) : Component(owner)
{
	EventManager::AddEvent("DIE",std::bind(&dae::GameManager::OnDie,this,m_Argument));
	EventManager::AddEvent("BURGERDONE",std::bind(&dae::GameManager::OnBurgderDone,this,m_Argument));
	EventManager::AddEvent("0BUTTON_A", std::bind(&dae::GameManager::OnSalt, this, m_Argument));

	m_healthImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "Health.png");
	m_pepperImage = ResourceManager::GetInstance().LoadTexture(m_BasePath + "pepper.png");

	InitialiseData();
	AddGameUI();
}

dae::GameManager::~GameManager()
{

}

void dae::GameManager::Update(float)
{
	m_WasPressingPepper = m_PressingPepper;
	m_PressingPepper = false;
}

void dae::GameManager::Start()
{
	m_playerController = SceneManager::GetInstance().FindComponent<dae::PlayerController>()->GetOwner();
	m_playerStartpos = m_playerController->GetPosition().GetPosition();

	m_enemySpawner = static_cast<EnemySpawner*>(SceneManager::GetInstance().FindComponent<EnemySpawner>());
}

void dae::GameManager::Restart()
{
	//SceneManager::GetInstance().SwitchScene("Level1.json");
	//remove all enemies and place player in start
	m_healthString = std::to_string(m_health);
	m_playerController->SetPosition(m_playerStartpos);
	if(m_enemySpawner != nullptr) m_enemySpawner->Restart();
	UIManager::GetInstance().UpdateUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
}

void dae::GameManager::GameOver()
{
	HandleScore();
	EventManager::RemoveEvent("0BUTTON_A");
	UIManager::GetInstance().ClearUI();
	SoundManager::GetInstance().PlaySound("GameOver.wav");
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManager::AddGameUI()
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

void dae::GameManager::InitialiseData()
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

void dae::GameManager::HandleScore() //The most basic type of saving lol
{
	if (m_score > m_HighScore)
	{
		m_HighScore = m_score;
	}
	std::ofstream out{m_SoloDataPath.c_str()};
	out << m_HighScore;
	out.close();
}

void dae::GameManager::OnDie(byte*)
{
	m_health--;
	if (m_health == 0)
		GameOver();
	else if (m_health > 0)
		Restart();
}

void dae::GameManager::OnWin()
{
	SoundManager::GetInstance().PlaySound("NextLevel.wav");
	EventManager::RemoveEvent("0BUTTON_A");
	UIManager::GetInstance().ClearUI();
	HandleScore();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void dae::GameManager::OnSalt(byte*)
{
	if (!m_WasPressingPepper && m_pepper > 0)
	{
		m_pepper--;
		m_pepperString = std::to_string(m_pepper);
		EventManager::SendEvent("PEPPER",0);
		UIManager::GetInstance().UpdateUI();
	}
	m_PressingPepper = true;
}

void dae::GameManager::OnBurgderDone(byte*)
{
	m_doneBurgers++;
	if (m_doneBurgers == 16)
	{
		OnWin();
	}
}

void dae::GameManager::AddScore(int score)
{
	m_score += score;
	m_scoreString = std::to_string(m_score);
	UIManager::GetInstance().UpdateUI();
}
