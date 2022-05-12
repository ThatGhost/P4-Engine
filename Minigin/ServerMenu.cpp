#include "MiniginPCH.h"
#include "ServerMenu.h"

#include "playfab/PlayFabError.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include <windows.h>

using namespace PlayFab;
using namespace ClientModels;

void OnError(const PlayFabError&, void*);
void OnLoginConfirm(const LoginResult&, void*);
void OnGetServer(const CurrentGamesResult&, void*);

dae::ServerMenu::ServerMenu(GameObject* owner) : Component(owner)
{
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
	PlayFabClientAPI::Update();
}

void OnError(const PlayFabError& error, void*)
{
	std::cout << error.GenerateErrorReport().c_str() << '\n';
}

void OnLoginConfirm(const LoginResult&, void*)
{
	std::cout << "Connected to playfab\n";
	return;

	CurrentGamesRequest request;
	request.BuildVersion = "1";
	request.GameMode = "default";
	request.pfRegion = Region::RegionEUWest;

	PlayFabClientAPI::GetCurrentGames(request,OnGetServer,OnError);
}

void OnGetServer(const CurrentGamesResult& result, void*)
{
	std::cout << result.GameCount << '\n';
}
