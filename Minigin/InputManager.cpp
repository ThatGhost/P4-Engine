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
	//controllers
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

	//keyboard
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			KeyPress(e.key.keysym.sym,1);
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
			EventManager::SendEvent(eventName);
		}
	}
}

void dae::InputManager::KeyPress(const SDL_Keycode key, int)
{
	switch (key)
	{
	case SDLK_SPACE: EventManager::SendEvent(std::to_string(m_KeyboardId) +"BUTTON_A");break;
	case SDLK_v: EventManager::SendEvent(std::to_string(m_KeyboardId)+"BUTTON_B");break;
	case SDLK_w: EventManager::SendEvent(std::to_string(m_KeyboardId)+"BUTTON_UP"); break;
	case SDLK_s: EventManager::SendEvent(std::to_string(m_KeyboardId)+"BUTTON_DOWN");break;
	case SDLK_a: EventManager::SendEvent(std::to_string(m_KeyboardId)+"BUTTON_LEFT");break;
	case SDLK_d: EventManager::SendEvent(std::to_string(m_KeyboardId)+"BUTTON_RIGHT");break;
	default:
		break;
	}
}
