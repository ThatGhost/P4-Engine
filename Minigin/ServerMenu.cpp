#include "MiniginPCH.h"
#include "ServerMenu.h"
#include "SceneManager.h"
#include "NetworkHandler.h"

#include "playfab/PlayFabError.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include <windows.h>

using namespace PlayFab;
using namespace ClientModels;
using namespace dae;

void OnError(const PlayFabError&, void*);
void OnLoginConfirm(const LoginResult&, void*);
void OnGetServer(const CurrentGamesResult&, void*);

static ServerMenu* serverMenu;

dae::ServerMenu::ServerMenu(GameObject* owner) : Component(owner)
{
	serverMenu = this;
	PlayFabSettings::staticSettings->titleId = "6797C";

	LoginWithCustomIDRequest request;
	request.CreateAccount = true;
	request.CustomId = "GettingStartedGuide";

	PlayFabClientAPI::LoginWithCustomID(request, OnLoginConfirm, OnError);
}

dae::ServerMenu::~ServerMenu()
{
}

void dae::ServerMenu::Update(float)
{
	if (m_switchScenes)
	{
		std::cout << "switched scenes\n";
		SceneManager::GetInstance().SwitchScene("Online.json");
		m_switchScenes = false;
	}

	PlayFabClientAPI::Update();
}

void OnError(const PlayFabError& error, void*)
{
	std::cout << error.GenerateErrorReport().c_str() << '\n';
}

void OnLoginConfirm(const LoginResult&, void*)
{
	std::cout << "Connected to playfab\n";

	//CurrentGamesRequest request;
	//request.BuildVersion = "1";
	//request.GameMode = "default";
	//request.pfRegion = Region::RegionEUWest;

	//PlayFabClientAPI::GetCurrentGames(request,OnGetServer,OnError);
	GameObject* go = new GameObject();

	NetworkHandler* networkhandler = static_cast<NetworkHandler*>(go->AddComponent<NetworkHandler>());
	networkhandler->SetNetworkDetails("127.0.0.1", 5522);
	networkhandler->ConnectToServer();

	SceneManager::GetInstance().GetScene(0)->Add(go);
	serverMenu->SetSwitchScene(true);
}

void OnGetServer(const CurrentGamesResult& result, void*)
{
	std::cout << result.GameCount << '\n';
}
