#include "MiniginPCH.h"
#include <algorithm>
#include "InputManager.h"
#include "imgui_impl_sdl.h"
#include "Command.h"
#include "GameObject.h"
#include "EventManager.h"

bool dae::InputManager::ProcessInput()
{
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		dwResult = XInputGetState(i, &m_CurrentControllerState);
		if (dwResult == ERROR_SUCCESS)
		{
			for (auto c : All)
				ButtonPress(c, i);
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			std::cout << e.key.keysym.sym << std::endl;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::ButtonPress(const ControllerButton b, int player)
{
	if (m_Commands.contains((int)b))
	{
		int e = m_CurrentControllerState.Gamepad.wButtons & (int)b;
		if (e != 0)
		{
			//eventmanager
			std::string eventName = ""+std::to_string(player);
			eventName += m_Commands[e];
			EventManager::SendEvent(eventName, 0.f);
		}
	}
}