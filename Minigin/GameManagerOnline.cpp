#include "MiniginPCH.h"
#include "GameManagerOnline.h"
#include "OnlinePlayerController.h"
#include "OnlineEnemyController.h"
#include "EventManager.h"

dae::GameManagerOnline::GameManagerOnline(GameObject* owner) : GameManager(owner),
m_mapObjects{}
{
	EventManager::AddEvent("GAMEOVER",std::bind(&dae::GameManagerOnline::GameOver,this));
	EventManager::AddEvent("BURGERDONE",std::bind(&dae::GameManagerOnline::AddBurger,this));
}

dae::GameManagerOnline::~GameManagerOnline()
{
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
			m_playerController->SetGood(amountofconnectedPlayers == 1);
			m_EnemyController->SetGood(amountofconnectedPlayers != 1);

			// Server is assigning us OUR id
			msg >> m_nPlayerID;
			std::cout << "first int = " << amountofconnectedPlayers << "\n";
			std::cout << "second int = " << m_nPlayerID << "\n";
			//std::cout << "Assigned Client ID = " << m_nPlayerID << "\n";

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
				std::cout << std::boolalpha << "excists in game world >> " << m_bWaitingForConnection << std::endl;
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
			m_mapObjects.insert_or_assign(desc.nUniqueID, desc);
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

	for (auto& other : m_mapObjects)
	{		
		if(other.first != m_nPlayerID)
			m_EnemyController->GetOwner()->SetPosition(glm::vec3(other.second.x,other.second.y,0));
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
	std::cout << "send update to server\n";
}

void dae::GameManagerOnline::GameOver()
{
}

void dae::GameManagerOnline::GameWon()
{
}

void dae::GameManagerOnline::AddBurger()
{
	m_burgers++;
	if (m_burgers == m_MaxBurgers)
	{
		GameWon();
	}
}
