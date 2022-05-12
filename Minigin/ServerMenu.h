#pragma once
#include "Component.h"
#include "playfab/PlayFabError.h"
#include "playfab/PlayFabClientDataModels.h"
#include "playfab/PlayFabClientApi.h"
#include "playfab/PlayFabSettings.h"
#include "playfab/PlayFabApiSettings.h"
#include <windows.h>

namespace dae
{
	class ServerMenu :	public Component
	{
	public:
		ServerMenu(GameObject*);
		~ServerMenu() override;

	private:

	};
}

