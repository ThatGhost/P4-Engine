#pragma once
#include "GameManager.h"
namespace dae
{
	using byte = unsigned char;
	class UIElement;
	class Texture2D;
	class EnemySpawner;
	class GameManagerCoop : public GameManager
	{
	public:
		GameManagerCoop(GameObject* owner);
		~GameManagerCoop() override;

		virtual void Update(float) override;
		virtual void Start() override;
		virtual void AddScore(int);
	private:

		virtual void OnDie() override;
		virtual void OnPepper()override;
		void OnSalt() {};
		virtual void OnBurgderDone()override;

		virtual void Restart()override;
		virtual void GameOver()override;
		virtual void OnWin()override;

		void AddGameUI();
		void InitialiseData();
		void HandleScore();

		GameObject* m_playerObject = nullptr;
		EnemySpawner* m_enemySpawner = nullptr;

		const std::string m_BasePath{ "..\\Data\\UI\\" };
		const int m_NormalHealth = 3;
		const int m_NormalPepper = 3;

		int m_HighScore = 0;
		bool m_PressingPepper = false;
		bool m_WasPressingPepper = false;
		glm::vec3 m_PlayerStartPos{};
		bool m_Dead = false;

		int m_Score = 0;
		int m_Health = m_NormalHealth;
		int m_Pepper = m_NormalPepper;
		int m_DoneBurgers{};

		//UI
		std::string m_1Up{ "1UP" };
		std::string m_Hi_Score{ "HI" };

		std::string m_PepperString{ "5" };
		std::string m_HealthString{ "3" };
		std::string m_HighScoreString{ "10000" };
		std::string m_ScoreString{ "0" };

		std::string m_AppDataPath{ "" };
		std::string m_CoopDataPath{ "" };

		Texture2D* m_PepperImage;
		Texture2D* m_HealthImage;

		const float m_FontSize{ 32 };
	};
}