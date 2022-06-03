#pragma once
#include "GameManager.h"
#include "Singleton.h"

namespace dae
{
	using byte = unsigned char;
	class UIElement;
	class Texture2D;
	class EnemySpawner;
	class GameManagerVersus final : public GameManager
	{
	public:
		GameManagerVersus(GameObject* owner);
		~GameManagerVersus() override;

		virtual void Update(float) override;
		virtual void Start() override;
	private:
		void Restart() override;
		void GameOver() override;
		void OnWin() override;

		void OnDie() override;
		void OnPepper() override;
		void OnBurgderDone() override;

		void AddGameUI();

		GameObject* m_PlayerController = nullptr;

		const std::string m_BasePath{ "..\\Data\\UI\\" };
		const int m_NormalHealth = 3;
		const int m_NormalPepper = 3;

		bool m_PressingPepper = false;
		bool m_WasPressingPepper = false;
		glm::vec3 m_PlayerStartpos{};

		int m_Health = m_NormalHealth;
		int m_Pepper = m_NormalPepper;
		int m_DoneBurgers{};

		//UI
		std::string m_pepperString{ "5" };
		std::string m_healthString{ "5" };

		Texture2D* m_PepperImage;
		Texture2D* m_HealthImage;

		const float m_FontSize{ 32 };
	};
}