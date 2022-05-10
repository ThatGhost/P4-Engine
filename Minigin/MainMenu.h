#pragma once
#include "Component.h"

namespace dae
{
	class Texture2D;
	class UIElement;
	using byte = unsigned char;

	class MainMenu : public Component
	{
	public:
		MainMenu(GameObject* owner);
		virtual ~MainMenu() override;


	private:
		const std::string basePath{"..\\Data\\UI\\"};

		Texture2D* m_Logo;
		Texture2D* m_Arrow;
		std::string m_Player1{"1    PLAYER"};
		std::string m_Player2{"2    PLAYERS"};

		UIElement* m_Arrow1;
		UIElement* m_Arrow2;

		byte* m_Argument;

		bool m_firstPos{ true };
		//callbacks
		void OnDown(byte*);
		void OnUp(byte*);
		void OnA(byte*);
	};
}

