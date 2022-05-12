#include "MiniginPCH.h"
#include "ServerMenu.h"

using namespace PlayFab;
using namespace ClientModels;

void OnError(const PlayFabError&, void* e);
void OnLoginConfirm(const LoginResult&, void*);

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

void OnError(const PlayFabError& error, void*)
{
	std::cout << error.GenerateErrorReport().c_str() << '\n';
}

void OnLoginConfirm(const LoginResult&, void*)
{
	std::cout << "Connected to playfab\n";
}
