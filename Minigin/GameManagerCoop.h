#pragma once
#include "GameManager.h"
namespace dae
{
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
		virtual void OnSalt()override;
		void OnSalt2() {};
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
		glm::vec3 m_playerStartpos{};
		bool m_daed = false;

		int m_score = 0;
		int m_health = m_NormalHealth;
		int m_pepper = m_NormalPepper;
		int m_doneBurgers{};

		//UI
		std::string m_1Up{ "1UP" };
		std::string m_Hi_Score{ "HI" };

		std::string m_pepperString{ "5" };
		std::string m_healthString{ "3" };
		std::string m_highScoreString{ "10000" };
		std::string m_scoreString{ "0" };

		std::string m_appDataPath{ "" };
		std::string m_CoopDataPath{ "" };

		Texture2D* m_pepperImage;
		Texture2D* m_healthImage;

		const float m_FontSize{ 32 };
	};
}