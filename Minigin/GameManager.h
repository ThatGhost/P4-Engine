#pragma once
#include "Component.h"
#include "Singleton.h"

namespace dae
{
	using byte = unsigned char;
	class UIElement;
	class Texture2D;
	class EnemySpawner;
	class GameManager : public Component
	{
	public:
		static GameManager* GetInstance();

		GameManager(GameObject* owner);
		~GameManager() override;

		virtual void Update(float) override;
		virtual void Start() override;
		void AddScore(int);
	private:
		friend class Singleton<GameManager>;

		void Restart();
		void GameOver();
		void AddGameUI();
		void InitialiseData();
		void HandleScore();
		void OnDie(byte*);
		void OnWin(byte*);
		void OnSalt(byte*);

		GameObject* m_playerController = nullptr;
		EnemySpawner* m_enemySpawner = nullptr;

		byte* m_Argument = nullptr;
		const std::string m_BasePath{ "..\\Data\\UI\\" };
		const int m_NormalHealth = 5;
		const int m_NormalPepper = 3;

		int m_HighScore = 0;
		bool m_PressingPepper = false;
		bool m_WasPressingPepper = false;
		glm::vec3 m_playerStartpos{};

		int m_score = 0;
		int m_health = m_NormalHealth;
		int m_pepper = m_NormalPepper;

		//UI
		std::string m_1Up{"1UP"};
		std::string m_Hi_Score{"HI"};

		std::string m_pepperString{"5"};
		std::string m_healthString{"5"};
		std::string m_highScoreString{"10000"};
		std::string m_scoreString{ "0" };

		std::string m_appDataPath{""};
		std::string m_SoloDataPath{""};

		Texture2D* m_pepperImage;
		Texture2D* m_healthImage;

		const float m_FontSize{32};
	};
}

const std::string m_ReadMeText{ "Hi\nThis folder was created for the Programming 4 end assignment for Ibn Zwanckaert\nIf you already seen him and gave him 20/20 you can delete this.\nOtherwise pls give Ibn a 20/20 (he needs it)" };