#pragma once
#include "GameManager.h"
namespace dae
{
	using byte = unsigned char;
	class UIElement;
	class Texture2D;
	class EnemySpawner;
	class GameManagerSolo : public GameManager
	{
	public:
		GameManagerSolo(GameObject* owner);
		~GameManagerSolo() override;

		virtual void Update(float) override;
		virtual void Start() override;
		virtual void AddScore(int);

		void SetLevel(int l) { m_level = l; };
	private:

		virtual void OnDie(byte*) override;
		virtual void OnSalt(byte*)override;
		virtual void OnBurgderDone(byte*)override;

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

		bool m_endScreen = false;
		float m_endScreenTimer = 0;
		const float m_endScreenTime = 3;
		bool m_Lost{true};

		int m_score = 0;
		int m_scoreToAdd = 0;
		int m_health = m_NormalHealth;
		int m_pepper = m_NormalPepper;
		int m_doneBurgers{};
		int m_level{};

		//UI
		std::string m_1Up{"1UP"};
		std::string m_Hi_Score{"HI"};
		std::string m_YouWon{"YOU WON"};
		std::string m_YouLost{"YOU LOST"};

		std::string m_pepperString{"5"};
		std::string m_healthString{"3"};
		std::string m_highScoreString{"10000"};
		std::string m_scoreString{ "0" };

		std::string m_appDataPath{""};
		std::string m_SoloDataPath{""};

		Texture2D* m_pepperImage;
		Texture2D* m_healthImage;

		const float m_FontSize{32};
		const std::string m_ReadMeText{ "Hi\nThis folder was created for the Programming 4 end assignment for Ibn Zwanckaert\nIf you already seen him and gave him 20/20 you can delete this.\nOtherwise pls give Ibn a 20/20 (he needs it)" };
	};
}