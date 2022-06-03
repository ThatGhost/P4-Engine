#include "MiniginPCH.h"

#if _WIN64
#define ENVIROMENT64

#include "GameManagerOnline.h"
#include "EventManager.h"
#include "UIManager.h"
#include "ResourceManager.h"

#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <fstream>

dae::GameManagerOnline::GameManagerOnline(GameObject* owner) : GameManager(owner),
m_MapObjects{}
{
	InitialiseData();

	//resources
	m_WhaitingImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\WhaitingScreen.png");
	m_HealthImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\Health.png");
	m_PepperImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\pepper.png");

	//events
	EventManager::AddEvent("GAMEOVER",std::bind(&dae::GameManagerOnline::KilledOther,this));
	EventManager::AddEvent("BURGERDONE",std::bind(&dae::GameManagerOnline::AddBurger,this));
	EventManager::AddEvent("0BUTTON_X", std::bind(&dae::GameManagerOnline::SendPepper, this));

	AddGameUI();

	UIManager::GetInstance().UpdateUI();
}
dae::GameManagerOnline::~GameManagerOnline()
{
	EventManager::RemoveEvent("GAMEOVER");
	EventManager::RemoveEvent("BURGERDONE");
	EventManager::RemoveEvent("0BUTTON_X");
	UIManager::GetInstance().ClearUI();
}

void dae::GameManagerOnline::Update(float)
{
	if (m_DirtyUI)
	{
		UIManager::GetInstance().UpdateUI();
		m_DirtyUI = false;
	}

	if (IsConnected())
	{
		HandleIncomingMessages();
		HandleGameWorld();
		HandleOutGoingMessages();
	}

	m_WasPressingPepper = m_IsPressingPepper;
	m_IsPressingPepper = false;
}
void dae::GameManagerOnline::Start()
{
	if (!Connect("127.0.0.1", 5522))
	{
		std::cout << "Failed to connect to game\n";
	}
}
void dae::GameManagerOnline::AddScore(int score)
{
	m_Score += score;
	m_ScoreString = std::to_string(m_Score);
	m_DirtyUI = true;
}
void dae::GameManagerOnline::Restart()
{

	m_health = m_NormalHealth;
	m_HealthString = std::to_string(m_health);
	
	m_Pepper = m_NormalPepper;
	m_PepperString = std::to_string(m_Pepper);

	m_Score = 0;
	m_ScoreString = std::to_string(m_Score);

	m_burgers = 0;
	m_WaitingForStart = true;

	m_PlayerController->SetIsPlaying(false);

	m_PlayerController->GetOwner()->SetPosition(m_PlayerStartPos);
	m_EnemyController->GetOwner()->SetPosition(m_EnemyStartPos);

	UIManager::GetInstance().ClearUI();
	AddGameUI();
	UIManager::GetInstance().UpdateUI();
}

void dae::GameManagerOnline::HandleIncomingMessages()
{
	while (!Incoming().empty())
	{
		auto msg = Incoming().pop_front().msg;
		//std::cout << (int)msg.header.id << '\n';
		switch (msg.header.id)
		{
		//Setup messages
		case(GameMsg::Client_Accepted):
		{
			msg;
			std::cout << "Server accepted client - you're in!\n";
			olc::net::message<GameMsg> msgs;
			msgs.header.id = GameMsg::Client_RegisterWithServer;
			msgs << m_DescPlayer;
			Send(msgs);
			break;
		}
		case(GameMsg::Server_HeartBeat):
		{
			olc::net::message<GameMsg> msgs;
			msgs.header.id = GameMsg::Server_HeartBeat;
			msgs << 1;
			Send(msgs);
			break;
		}
		case(GameMsg::Client_AssignID):
		{
			//check if we are the good or bad guy
			uint32_t amountofconnectedPlayers;
			msg >> amountofconnectedPlayers;
			if (amountofconnectedPlayers == 1) //first player
			{
			//i am white
				m_Good = true;
				m_PlayerController->SetGood(true);
				m_EnemyController->SetGood(false);
			}
			else
			{
			//i am red
				m_Good = false;
				m_PlayerController->SetGood(false);
				m_EnemyController->SetGood(true);
			}

			// Server is assigning us OUR id
			msg >> m_NPlayerID;
			break;
		}
		case(GameMsg::Game_AddPlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			m_MapObjects.insert_or_assign(desc.nUniqueID, desc);

			if (desc.nUniqueID == m_NPlayerID)
			{
				// Now we exist in game world
				m_WaitingForConnection = false;
				m_PlayerController->GetOwner()->SetPosition(glm::vec3(desc.x,desc.y,5));
			}
			else
			{
				m_EnemyController->GetOwner()->SetPosition(glm::vec3(desc.x,desc.y,5));
			}
			break;
		}
		case(GameMsg::Game_RemovePlayer):
		{
			uint32_t nRemovalID = 0;
			msg >> nRemovalID;
			m_MapObjects.erase(nRemovalID);
			Restart();
			break;
		}
		case(GameMsg::Game_UpdatePlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			if (desc.nUniqueID != m_NPlayerID)
			{
				m_MapObjects.insert_or_assign(desc.nUniqueID, desc);
			}
			break;
		}

		//Manager messages
		case(GameMsg::Game_Lose):
		{
			GameOver();
			break;
		}
		case(GameMsg::Game_Start):
		{
			m_PlayerController->SetIsPlaying(true);
			m_WaitingForStart = false;
			m_WhaitingElement->SetActive(false);
			UIManager::GetInstance().UpdateUI();
			break;
		}
		case(GameMsg::Game_Restart):
		{
			//restart
			int i;
			msg >> i;
			RoundReset(i);
			break;
		}

		//Runtime messages
		case(GameMsg::Game_Pepper):
		{
			//pepper
			int i;
			msg >> i;
			GotPepper(i);
			break;
		}
		}
	}
}
void dae::GameManagerOnline::HandleGameWorld()
{
	glm::vec3 playerPosition = m_PlayerController->GetOwner()->GetPosition().GetPosition();
	m_DescPlayer.x = playerPosition.x;
	m_DescPlayer.y = playerPosition.y;
	m_DescPlayer.state = m_PlayerController->GetState();

	for (auto& other : m_MapObjects)
	{		
		if (other.first != m_NPlayerID)
		{
			m_EnemyController->GetOwner()->SetPosition(glm::vec3(other.second.x, other.second.y, 5));
			m_EnemyController->SetState(other.second.state);
		}
	}
}
void dae::GameManagerOnline::HandleOutGoingMessages()
{
	if (m_WaitingForConnection)
		return;

	olc::net::message<GameMsg> msgs;
	msgs.header.id = GameMsg::Game_UpdatePlayer;
	msgs << m_DescPlayer;
	Send(msgs);
}

void dae::GameManagerOnline::GameOver()
{
	m_PlayerController->SetIsPlaying(false);
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(60, 200), SDL_Color(210, 50, 50));
	UIManager::GetInstance().UpdateUI();
}
void dae::GameManagerOnline::GameWon()
{
	m_PlayerController->SetIsPlaying(false);
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(60, 200), SDL_Color(50, 210, 50));
	UIManager::GetInstance().UpdateUI();
	
	//restart when pressing x
}
void dae::GameManagerOnline::KilledOther()
{
	m_health--;

	if (m_health == 0)
	{
		//kill
		GameWon();
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_Lose;
		msg << 1;
		Send(msg);
	}
	else
	{
		//restart
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_Restart;
		msg << m_health;
		Send(msg);
	}

}
void dae::GameManagerOnline::SendPepper()
{
	if (!m_Good)
		return;

	if (!m_WasPressingPepper && m_Pepper > 0)
	{
		//ui
		m_Pepper--;
		m_PepperString = std::to_string(m_Pepper);
		m_DirtyUI = true;
		//event
		EventManager::SendEvent("PEPPER");
		//net code
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_Pepper;
		msg << m_Pepper;
		Send(msg);
	}
	m_IsPressingPepper = true;
}
void dae::GameManagerOnline::GotPepper(int peppers)
{
	if (m_Good)
		return;

	m_Pepper = peppers;
	m_PepperString = std::to_string(m_Pepper);
	m_DirtyUI = true;

	m_EnemyController->ShootPepper();
}
void dae::GameManagerOnline::AddBurger()
{
	m_burgers++;

	if (m_burgers == m_MaxBurgers)
	{
		GameWon();
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_Lose;
		Send(msg);
	}
}
void dae::GameManagerOnline::RoundReset(int hp)
{
	m_health = hp;
	m_HealthString = std::to_string(hp);
	m_DirtyUI = true;

	m_PlayerController->GetOwner()->SetPosition(m_PlayerStartPos);
	m_EnemyController->GetOwner()->SetPosition(m_EnemyStartPos);
}
void dae::GameManagerOnline::InitialiseData()
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
	std::ofstream out{ m_AppDataPath + "ReadMe.txt" };
	out << m_ReadMeText;
	out.close();

	//create data folder path
	m_SoloDataPath = szPath;
	m_SoloDataPath += "\\ONLINE\\";
	CreateDirectory(m_SoloDataPath.c_str(), NULL);
	m_SoloDataPath += "ScoreData.dat";

	//make score file if it doesn't exsist
	std::ifstream in{ m_SoloDataPath };
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
			m_HighScoreString = score;
		}
	}

	in.close();
}

void dae::GameManagerOnline::AddGameUI()
{
	//--UI--
//score
	UIManager::GetInstance().AddTextElement(&m_1Up, m_FontSize, glm::vec2(20, 10), SDL_Color(0, 255, 0));
	UIManager::GetInstance().AddTextElement(&m_ScoreString, m_FontSize - 10, glm::vec2(90, 20));

	//Hi score
	UIManager::GetInstance().AddTextElement(&m_Hi_Score, m_FontSize, glm::vec2(210, 10), SDL_Color(255, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_HighScoreString, m_FontSize - 10, glm::vec2(250, 20));

	//pepper
	UIManager::GetInstance().AddImageElement(&m_PepperImage, glm::vec2(32, 32), glm::vec2(365, 15));
	UIManager::GetInstance().AddTextElement(&m_PepperString, m_FontSize - 10, glm::vec2(395, 20));

	//Health
	UIManager::GetInstance().AddImageElement(&m_HealthImage, glm::vec2(32, 32), glm::vec2(425, 12));
	UIManager::GetInstance().AddTextElement(&m_HealthString, m_FontSize - 10, glm::vec2(455, 20));

	m_WhaitingElement = UIManager::GetInstance().AddImageElement(&m_WhaitingImage, glm::vec2(500, 580), glm::vec2(0, 0));
}

#endif