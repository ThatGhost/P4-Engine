#include "MiniginPCH.h"
#include <algorithm>
#include "InputManager.h"
#include "imgui_impl_sdl.h"
#include "GameObject.h"
#include "EventManager.h"

void dae::InputManager::Init()
{
	InsertCommand(ControllerButton::ButtonA,"BUTTON_A");
	InsertCommand(ControllerButton::ButtonB,"BUTTON_B");
	InsertCommand(ControllerButton::ButtonX,"BUTTON_X");
	InsertCommand(ControllerButton::ButtonY,"BUTTON_Y");

	InsertCommand(ControllerButton::ButtonUp,"BUTTON_UP");
	InsertCommand(ControllerButton::ButtonDown,"BUTTON_DOWN");
	InsertCommand(ControllerButton::ButtonLeft,"BUTTON_LEFT");
	InsertCommand(ControllerButton::ButtonRight,"BUTTON_RIGHT");

	InsertCommand(ControllerButton::ButtonLeftShoulder,"L_SHOULDER");
	InsertCommand(ControllerButton::ButtonRightShoulder,"R_SHOULDER");

	InsertCommand(ControllerButton::ButtonLeftThumb,"L_THUMB");
	InsertCommand(ControllerButton::ButtonRightThumb,"R_THUMB");

	InsertCommand(ControllerButton::ButtonStart,"BUTTON_START");
	InsertCommand(ControllerButton::ButtonBack,"BUTTON_BACK");
}

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
			std::string eventName = std::to_string(player);
			eventName += m_Commands[e];
			EventManager::SendEvent(eventName, 0);
		}
	}
}