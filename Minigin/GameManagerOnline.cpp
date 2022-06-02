#include "MiniginPCH.h"
#include "GameManagerOnline.h"
#include "OnlinePlayerController.h"
#include "OnlineEnemyController.h"
#include "EventManager.h"
#include "UIManager.h"
#include "ResourceManager.h"

dae::GameManagerOnline::GameManagerOnline(GameObject* owner) : GameManager(owner),
m_mapObjects{}
{
	//resources
	m_healthImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\WhaitingScreen.png");

	//events
	EventManager::AddEvent("GAMEOVER",std::bind(&dae::GameManagerOnline::KilledOther,this));
	EventManager::AddEvent("BURGERDONE",std::bind(&dae::GameManagerOnline::AddBurger,this));

	//UI
	m_WhaitingElement = UIManager::GetInstance().AddImageElement(&m_healthImage,glm::vec2(500,580),glm::vec2(0,0));
	UIManager::GetInstance().UpdateUI();
}

dae::GameManagerOnline::~GameManagerOnline()
{
	UIManager::GetInstance().ClearUI();
}

void dae::GameManagerOnline::Update(float)
{
	if (IsConnected())
	{
		HandleIncomingMessages();
		HandleGameWorld();
		HandleOutGoingMessages();
	}
}

void dae::GameManagerOnline::Start()
{
	if (!Connect("127.0.0.1", 5522))
	{
		std::cout << "Failed to connect to game\n";
	}
}

void dae::GameManagerOnline::HandleIncomingMessages()
{
	while (!Incoming().empty())
	{
		auto msg = Incoming().pop_front().msg;

		switch (msg.header.id)
		{
		case(GameMsg::Client_Accepted):
		{
			msg;
			std::cout << "Server accepted client - you're in!\n";
			olc::net::message<GameMsg> msgs;
			msgs.header.id = GameMsg::Client_RegisterWithServer;
			msgs << m_descPlayer;
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
				m_playerController->SetGood(true);
				m_EnemyController->SetGood(false);
			}
			else
			{
			//i am red
				m_playerController->SetGood(false);
				m_EnemyController->SetGood(true);
			}

			// Server is assigning us OUR id
			msg >> m_nPlayerID;
			break;
		}

		case(GameMsg::Game_AddPlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			m_mapObjects.insert_or_assign(desc.nUniqueID, desc);

			if (desc.nUniqueID == m_nPlayerID)
			{
				// Now we exist in game world
				m_bWaitingForConnection = false;
				m_playerController->GetOwner()->SetPosition(glm::vec3(desc.x,desc.y,5));
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
			m_mapObjects.erase(nRemovalID);
			break;
		}

		case(GameMsg::Game_UpdatePlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			if (desc.nUniqueID != m_nPlayerID)
			{
				m_mapObjects.insert_or_assign(desc.nUniqueID, desc);
			}
			break;
		}

		//my messages
		case(GameMsg::Game_Lose):
		{
			GameOver();
			break;
		}
		case(GameMsg::Game_Start):
		{
			m_playerController->SetIsPlaying(true);
			m_bWaitingForStart = false;
			m_WhaitingElement->SetActive(false);
			UIManager::GetInstance().UpdateUI();
			break;
		}
		case(GameMsg::Game_Stop):
		{
			//disconnect from server
			//go back to server browser
			break;
		}
		case(GameMsg::Game_Restart):
		{
			//restart

			break;
		}
		}
	}
}

void dae::GameManagerOnline::HandleGameWorld()
{
	glm::vec3 playerPosition = m_playerController->GetOwner()->GetPosition().GetPosition();
	m_descPlayer.x = playerPosition.x;
	m_descPlayer.y = playerPosition.y;
	m_descPlayer.state = m_playerController->GetState();

	for (auto& other : m_mapObjects)
	{		
		if (other.first != m_nPlayerID)
		{
			m_EnemyController->GetOwner()->SetPosition(glm::vec3(other.second.x, other.second.y, 5));
			m_EnemyController->SetState(other.second.state);
		}
	}
}

void dae::GameManagerOnline::HandleOutGoingMessages()
{
	if (m_bWaitingForConnection)
		return;

	olc::net::message<GameMsg> msgs;
	msgs.header.id = GameMsg::Game_UpdatePlayer;
	msgs << m_descPlayer;
	Send(msgs);
}

void dae::GameManagerOnline::GameOver()
{
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouLost, 64, glm::vec2(60, 200), SDL_Color(210, 50, 50));
	UIManager::GetInstance().UpdateUI();
}

void dae::GameManagerOnline::GameWon()
{
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(70, 210), SDL_Color(0, 0, 0));
	UIManager::GetInstance().AddTextElement(&m_YouWon, 64, glm::vec2(60, 200), SDL_Color(50, 210, 50));
	UIManager::GetInstance().UpdateUI();
	
	//restart when pressing x
}
void dae::GameManagerOnline::KilledOther()
{
	GameWon();
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_Lose;
	Send(msg);
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