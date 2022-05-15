#include "MiniginPCH.h"
#include "NetworkHandler.h"
#include "SceneManager.h"
#include "PlayerController.h"

dae::NetworkHandler::NetworkHandler(GameObject* owner) : Component(owner)
{

}

dae::NetworkHandler::~NetworkHandler()
{

}

void dae::NetworkHandler::Update(float)
{
	if (IsConnected())
	{
		//incoming messages
		HandleIncomingMessages();
		//outgoing messages
		HandleOutGoingMessages();
	}
}

void dae::NetworkHandler::Start()
{
	m_player = SceneManager::GetInstance().FindComponent<PlayerController>()->GetOwner();
}

void dae::NetworkHandler::ConnectToServer()
{
	Connect(m_ipv4, m_port);
}

void dae::NetworkHandler::HandleIncomingMessages()
{
	while (!Incoming().empty())
	{
		auto msg = Incoming().pop_front().msg;

		switch (msg.header.id)
		{
		case(GameMsg::Client_Accepted):
		{
			std::cout << "Server accepted client - you're in!\n";		
			olc::net::message<GameMsg> msgOut;
			msgOut.header.id = GameMsg::Client_RegisterWithServer;
			Send(msgOut);
			break;
		}

		case(GameMsg::Client_AssignID):
		{
			// Server is assigning us OUR id
			msg >> nPlayerID;
			std::cout << "Assigned Client ID = " << nPlayerID << "\n";
			break;
		}

		case(GameMsg::Game_AddPlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			mapObjects.insert_or_assign(desc.nUniqueID, desc);
			std::cout << desc.x << ", " << desc.y << '\n';
			if (desc.nUniqueID == nPlayerID)
			{
				// Now we exist in game world
				bWaitingForConnection = false;
			}
			break;
		}

		case(GameMsg::Game_RemovePlayer):
		{
			uint32_t nRemovalID = 0;
			msg >> nRemovalID;
			mapObjects.erase(nRemovalID);
			break;
		}

		case(GameMsg::Game_UpdatePlayer):
		{
			sPlayerDescription desc;
			msg >> desc;
			mapObjects.insert_or_assign(desc.nUniqueID, desc);
			break;
		}
		}
	}
}

void dae::NetworkHandler::HandleOutGoingMessages()
{
	
}
