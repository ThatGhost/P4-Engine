#pragma once
#include <XInput.h>
#include <vector>
#include "Singleton.h"

namespace dae
{
	enum class ControllerButton
	{
		ButtonA = XINPUT_GAMEPAD_A,
		ButtonB = XINPUT_GAMEPAD_B,
		ButtonX = XINPUT_GAMEPAD_X,
		ButtonY = XINPUT_GAMEPAD_Y,
		ButtonLeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
		ButtonRightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		ButtonUp = XINPUT_GAMEPAD_DPAD_UP,
		ButtonDown = XINPUT_GAMEPAD_DPAD_DOWN,
		ButtonLeft = XINPUT_GAMEPAD_DPAD_LEFT,
		ButtonRight = XINPUT_GAMEPAD_DPAD_RIGHT,
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsDown(int player,ControllerButton button) const;
		bool IsPressed(int player,ControllerButton button) const;
		bool IsReleased(int player,ControllerButton button) const;
	private:
		std::vector<XINPUT_STATE> m_CurrentControllerState{};
		std::vector<XINPUT_STATE> m_PrevControllerState{};
		XINPUT_STATE m_CurrentKeyBoardState{};
		XINPUT_STATE m_PrevKeyBoardState{};
	};

}
