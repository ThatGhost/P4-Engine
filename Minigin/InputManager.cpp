#include "MiniginPCH.h"
#include "InputManager.h"
#include "imgui_impl_sdl.h"

bool dae::InputManager::ProcessInput()
{
	if (m_CurrentControllerState.size() != XUSER_MAX_COUNT)
	{
		m_CurrentControllerState.resize(XUSER_MAX_COUNT);
		m_PrevControllerState.resize(XUSER_MAX_COUNT);
	}

	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		m_PrevControllerState = m_CurrentControllerState;
		dwResult = XInputGetState(i, &m_CurrentControllerState[i]);
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

bool dae::InputManager::IsDown(int player,ControllerButton button) const
{
	return m_CurrentControllerState[player].Gamepad.wButtons & (int)button;
}

bool dae::InputManager::IsPressed(int player, ControllerButton button) const
{
	auto changes = m_CurrentControllerState[player].Gamepad.wButtons ^ m_PrevControllerState[player].Gamepad.wButtons;
	return changes & (int)button;
}

bool dae::InputManager::IsReleased(int player, ControllerButton button) const
{
	auto changes = m_CurrentControllerState[player].Gamepad.wButtons ^ m_PrevControllerState[player].Gamepad.wButtons;
	return changes & (~(int)button);
}


