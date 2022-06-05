#include "MiniginPCH.h"
#include "ServerBrowserComponent.h"

#if _WIN64

#include "ResourceManager.h"
#include "SceneManager.h"
#include <fstream>

dae::ServerBrowserComponent* g_serverBrowser;

dae::ServerBrowserComponent::~ServerBrowserComponent()
{

}

void dae::ServerBrowserComponent::Start()
{
	//Load textures
	m_ServerBackGround = ResourceManager::GetInstance().LoadTexture(m_BasePath+"ServerBg.png");
	m_ServerIcon = ResourceManager::GetInstance().LoadTexture(m_BasePath+"ServerIcon.png");
	m_ServerSelector = ResourceManager::GetInstance().LoadTexture(m_BasePath+"Arrow.png");

	//construct UI
	UIManager::GetInstance().AddTextElement(&m_ServerText, 32, glm::vec2(m_Margin.x, 10));

	//for (size_t i = 0; i < 3; i++)
	//{
	//	ServerDetails d{};
	//	d.Name = "server" + i;

	//	m_Servers.push_back(d);
	//	ConstructContainer(d);
	//}

	UIManager::GetInstance().UpdateUI();

	//playfab
	g_serverBrowser = this;
	LoadPlayfab();
}

void dae::ServerBrowserComponent::ConstructContainer(const ServerDetails& details)
{
	ContainerElement* container = static_cast<ContainerElement*>(UIManager::GetInstance().AddContainer(glm::vec2()));

	glm::vec2 pos{};
	pos.x = m_Margin.x;
	pos.y = m_TopDistance + (m_BlockSize.y + m_Margin.y) * m_Servers.size();

	m_Selectors.push_back(UIManager::GetInstance().AddImageElement(&m_ServerSelector,glm::vec2(32,32), glm::vec2(m_Margin.x - 32, pos.y + (m_BlockSize.y/2-16))));
	if (m_Selectors.size() > 1)
	{
		m_Selectors[m_Selectors.size() - 1]->SetActive(false);
	}

	container->AddElement(std::make_shared<ImageElement>(&m_ServerBackGround,m_BlockSize,pos));
	container->AddElement(std::make_shared<ImageElement>(&m_ServerIcon,m_IconSize, pos + m_IconMargin));
	//container->AddElement(std::make_shared<TextElement>(&details->Name,(float)25,pos + glm::vec2(80,25),SDL_Color(70,220,70)));
	container->AddElement(std::make_shared<TextElement>(&m_ServerText,(float)25,pos + glm::vec2(80,25),SDL_Color(70,220,70)));
	details;
}

#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include "playfab/PlayFabError.h"
#include "playfab/PlayFabMultiplayerApi.h"
#include "playfab/PlayFabMultiplayerDataModels.h"
#include <windows.h>

using namespace PlayFab;
using namespace ClientModels;
using namespace MultiplayerModels;

void PlayFabErrorCallBack(const PlayFab::PlayFabError& error, void*)
{
	std::cout << "[ERROR]: " << error.ErrorMessage << '\n';
}
void OnLoginSuccess(const LoginResult& result, void* customData);
void GetCurrentGamesCallBack(const CreateMatchmakingTicketResult& result, void*);
void OnTicketPoll(const GetMatchmakingTicketResult& result, void*);
void GetMatchCallBack(const GetMatchResult& result, void*);

void dae::ServerBrowserComponent::Update(float deltaTime)
{
	PlayFabClientAPI::Update();

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
	g_serverBrowser->ChangeServerText(result.Status);
	if (result.Status == "Matched")
	{
		//match
		g_serverBrowser->StopPolling();

		GetMatchRequest request;
		request.QueueName = "Normal";
		request.MatchId = result.MatchId;

		PlayFabMultiplayerAPI::GetMatch(request,GetMatchCallBack,PlayFabErrorCallBack);
	}
}

#include "MatchComponent.h"
void GetMatchCallBack(const GetMatchResult& result, void*)
{
	//get match details
	MultiplayerModels::ServerDetails details = result.pfServerDetails;

	uint16_t port = 0;
	for (auto ports : details.Ports)
	{
		port = (uint16_t)ports.Num;
	}


	//construct GO
	dae::GameObject* serverDetailHolder = new dae::GameObject();
	dae::SceneManager::GetInstance().DontDestroyOnLoad(serverDetailHolder);
	dae::MatchComponent* matchc = static_cast<dae::MatchComponent*>( serverDetailHolder->AddComponent<dae::MatchComponent>());
	matchc->Init(details.IPV4Address,port);

	//switch scene
	dae::SceneManager::GetInstance().SwitchScene("Online1.json");
}

#endif