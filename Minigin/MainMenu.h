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

		virtual void Update(float) override;

	private:
		void handleInput();
		void DisableAllArrows();

		const std::string m_BasePath{"..\\Data\\UI\\"};

		Texture2D* m_Logo;
		Texture2D* m_Arrow;
		std::string m_Player1{"1    PLAYER"};
		std::string m_Player2{"2    PLAYERS"};
		std::string m_Online{"ONLINE"};
		std::string m_Coop{"CO-OP"};

		std::vector<UIElement*> m_Arrows{};

		enum class MenuPosition
		{
			solo,
			versus,
			coop,
			online,
			end
		};
		MenuPosition m_MenuPosition{ MenuPosition::solo };
		bool m_WasPressed{false};
		bool m_IsPressing{ false };
		bool m_IsDown{ false };

		//callbacks
		void OnDown();
		void OnUp();
		void OnA();
	};
}

