#include "MiniginPCH.h"
#include "ServerBrowserComponent.h"

#if _WIN64
#define ENVIROMENT64

#include "ResourceManager.h"
#include "SceneManager.h"
#include "UIManager.h"
#include <fstream>
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include "playfab/PlayFabError.h"
#include "playfab/PlayFabMultiplayerApi.h"
#include "playfab/PlayFabMultiplayerDataModels.h"
#include <windows.h>
#include "MatchComponent.h"
using namespace PlayFab;
using namespace ClientModels;
using namespace MultiplayerModels;

dae::ServerBrowserComponent* g_serverBrowser;
void PlayFabErrorCallBack(const PlayFab::PlayFabError& error, void*)
{
	std::cout << "[ERROR]: " << error.ErrorMessage << '\n';
}
void OnLoginSuccess(const LoginResult& result, void* customData);
void GetCurrentGamesCallBack(const CreateMatchmakingTicketResult& result, void*);
void OnTicketPoll(const GetMatchmakingTicketResult& result, void*);
void GetMatchCallBack(const GetMatchResult& result, void*);
void CancelMatchCallBack(const CancelMatchmakingTicketResult& result, void*);

dae::ServerBrowserComponent::~ServerBrowserComponent()
{
	if (m_TicketId != "NULL" && m_UngracefullDisconnect)
	{
		CancelMatchmakingTicketRequest request;
		request.QueueName = "Normal";
		request.TicketId = m_TicketId;
		PlayFabMultiplayerAPI::CancelMatchmakingTicket(request, CancelMatchCallBack,PlayFabErrorCallBack);
	}
}
void dae::ServerBrowserComponent::Start()
{
	//construct UI
	m_ServerElement1 = UIManager::GetInstance().AddTextElement(&m_ServerText, 32, glm::vec2(m_Margin.x, m_Margin.y));
	m_ServerElement2 = UIManager::GetInstance().AddTextElement(&m_ServerText2, 32, glm::vec2(m_Margin.x + 50, m_Margin.y + 40));

	m_SearchElement1 = UIManager::GetInstance().AddTextElement(&m_SearchText, 32, glm::vec2(m_Margin.x, m_Margin.y));
	m_SearchElement2 = UIManager::GetInstance().AddTextElement(&m_SearchText2, 32, glm::vec2(m_Margin.x + 50, m_Margin.y + 40));

	m_SearchElement1->SetActive(false);
	m_SearchElement2->SetActive(false);

	UIManager::GetInstance().UpdateUI();

	//playfab
	g_serverBrowser = this;
	LoadPlayfab();
}
void dae::ServerBrowserComponent::SwtichText()
{
	m_SearchElement1->SetActive(true);
	m_SearchElement2->SetActive(true);

	m_ServerElement1->SetActive(false);
	m_ServerElement2->SetActive(false);

	m_UIDirty = true;
}
void dae::ServerBrowserComponent::FoundMatch(const std::string& ip, uint16_t port)
{
	m_UngracefullDisconnect = false;
	m_FoundMatch = true;
	m_Ipv4 = ip;
	m_Port = port;
}
void dae::ServerBrowserComponent::Update(float deltaTime)
{
	PlayFabClientAPI::Update();

	if (m_UIDirty)
	{
		UIManager::GetInstance().UpdateUI();
		m_UIDirty = false;
	}
	if (m_FoundMatch)
	{
		//construct GO
		dae::GameObject* serverDetailHolder = new dae::GameObject();
		dae::SceneManager::GetInstance().DontDestroyOnLoad(serverDetailHolder);
		dae::MatchComponent* matchc = static_cast<dae::MatchComponent*>(serverDetailHolder->AddComponent<dae::MatchComponent>());
		matchc->Init(m_Ipv4, m_Port);
		dae::SceneManager::GetInstance().SwitchScene("Online1.json");
		m_FoundMatch = false;
	}

	if (m_Poll)
	{
		if (m_PollTimer > m_PollTime)
		{
			m_PollTimer = 0;
			//poll
			GetMatchmakingTicketRequest request;
			request.TicketId = m_TicketId;
			request.QueueName = "Normal";

			PlayFabMultiplayerAPI::GetMatchmakingTicket(request,OnTicketPoll,PlayFabErrorCallBack);
		}
		else
		{
			m_PollTimer += deltaTime;
		}
	}

}
void dae::ServerBrowserComponent::LoadPlayfab()
{
	srand((unsigned int)time(NULL));

	PlayFabSettings::staticSettings->titleId = ("6797C");

	LoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = std::to_string(rand()%10000);
	//request.CustomId = std::to_string(10000);

	PlayFabClientAPI::LoginWithCustomID(request, OnLoginSuccess, PlayFabErrorCallBack);
}

void dae::ServerBrowserComponent::GoBack()
{
	UIManager::GetInstance().ClearUI();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}

void OnLoginSuccess(const LoginResult& result, void*)
{
	std::cout << "login succes\n";

	//static params
	CreateMatchmakingTicketRequest request;
	request.GiveUpAfterSeconds = 120;
	request.QueueName = "Normal";

	//player params
	MatchmakingPlayer player;
	EntityTokenResponse response = result.EntityToken;
	ClientModels::EntityKey key = response.Entity;

	MultiplayerModels::EntityKey ckey;
	ckey.Id = key.Id;
	ckey.Type = key.Type;

	//attributes
	MultiplayerModels::MatchmakingPlayerAttributes atributes = MultiplayerModels::MatchmakingPlayerAttributes();
	Json::Value root;
	//root["latencies"][0]["NorthEurope"] = 10;
	root["latencies"][0]["region"] = "NorthEurope";
	root["latencies"][0]["latency"] = 50;
	atributes.DataObject = root;

	//final
	player.Attributes = atributes;
	player.Entity = ckey;
	request.Creator = player;
	PlayFabMultiplayerAPI::CreateMatchmakingTicket(request,GetCurrentGamesCallBack,PlayFabErrorCallBack);
}
void GetCurrentGamesCallBack(const CreateMatchmakingTicketResult& result, void*)
{
	g_serverBrowser->StartPolling(result.TicketId);
}
void OnTicketPoll(const GetMatchmakingTicketResult& result, void*)
{
	if (result.Status == "Matched")
	{
		//match
		g_serverBrowser->StopPolling();

		GetMatchRequest request;
		request.QueueName = "Normal";
		request.MatchId = result.MatchId;

		PlayFabMultiplayerAPI::GetMatch(request,GetMatchCallBack,PlayFabErrorCallBack);
	}
	else
	{
		std::cout << result.Status << '\n';
		g_serverBrowser->SwtichText();
	}
}

void GetMatchCallBack(const GetMatchResult& result, void*)
{
	//get match details
	MultiplayerModels::ServerDetails details = result.pfServerDetails;

	uint16_t port = 0;
	for (auto ports : details.Ports)
	{
		port = (uint16_t)ports.Num;
	}

	//switch scene
	g_serverBrowser->FoundMatch(details.IPV4Address,port);
}
void CancelMatchCallBack(const CancelMatchmakingTicketResult&, void*) {}
#endif