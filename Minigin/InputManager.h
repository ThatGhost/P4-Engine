#pragma once
#include <XInput.h>
#include <vector>
#include <map>
#include "Singleton.h"

namespace dae
{
	class Command;
	class GameObject;

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
		ButtonLeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		ButtonRightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		ButtonStart = XINPUT_GAMEPAD_START,
		ButtonBack = XINPUT_GAMEPAD_BACK
	};

	static const ControllerButton All[]{
	ControllerButton::ButtonA,
	ControllerButton::ButtonB,
	ControllerButton::ButtonX,
	ControllerButton::ButtonY,
	ControllerButton::ButtonLeftShoulder,
	ControllerButton::ButtonRightShoulder,
	ControllerButton::ButtonUp,
	ControllerButton::ButtonDown,
	ControllerButton::ButtonLeft,
	ControllerButton::ButtonRight,
	ControllerButton::ButtonLeftThumb,
	ControllerButton::ButtonRightThumb,
	ControllerButton::ButtonStart,
	ControllerButton::ButtonBack
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Init();
		void InsertCommand(dae::ControllerButton button, const std::string& eventName)
		{
			if(!m_Commands.contains((int)button))
				m_Commands.insert(std::pair<int, std::string>((int)button, eventName));
		}
		bool ProcessInput();
		void SetKeyboardId(int id) { m_keyboardId = id; }
	private:
		XINPUT_STATE m_CurrentControllerState{};
		std::map<int,std::string> m_Commands;
		const float DEAD_ZONE{ 0.4f };
		int m_keyboardId{0};

		void ButtonPress(const ControllerButton, int);
		void KeyPress(const SDL_Keycode, int);
	};
}
