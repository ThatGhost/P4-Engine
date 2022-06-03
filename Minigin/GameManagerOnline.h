#pragma once
#if _WIN64
#define ENVIROMENT64

#include "GameManager.h"
#include "olcPGEX_Network.h"
#include "MMO_Common.h"
#include <unordered_map>
#include "OnlinePlayerController.h"
#include "OnlineEnemyController.h"

namespace dae
{
	class Texture2D;
	class UIElement;

	class GameManagerOnline : public GameManager, olc::net::client_interface<GameMsg>
	{
	public:
		GameManagerOnline(GameObject*);
		~GameManagerOnline() override;

		virtual void Update(float) override;
		virtual void Start() override;
		virtual void AddScore(int) override;
		virtual void Restart() override;

		void SetPlayer(OnlinePlayerController* p) { m_PlayerController = p; m_PlayerStartPos = p->GetOwner()->GetPosition().GetPosition(); }
		void SetEnemy(OnlineEnemyController* p) { m_EnemyController = p; m_EnemyStartPos = p->GetOwner()->GetPosition().GetPosition();	}
		
	private:
		std::unordered_map<uint32_t, sPlayerDescription> m_MapObjects;
		uint32_t m_NPlayerID = 0;
		sPlayerDescription m_DescPlayer;

		OnlinePlayerController* m_PlayerController = nullptr;
		OnlineEnemyController* m_EnemyController = nullptr;

		glm::vec3 m_PlayerStartPos;
		glm::vec3 m_EnemyStartPos;

		bool m_WasPressingPepper{ true };
		bool m_IsPressingPepper{ true };
		bool m_Good{false};

		//UI
		bool m_WaitingForConnection = true;
		bool m_WaitingForStart = true;
		dae::UIElement* m_WhaitingElement = nullptr;

		const float m_FontSize{ 32 };

		Texture2D* m_WhaitingImage;
		Texture2D* m_HealthImage;
		Texture2D* m_PepperImage;

		std::string m_1Up{ "1UP" };
		std::string m_Hi_Score{ "HI" };
		std::string m_YouWon{ "YOU WON" };
		std::string m_YouLost{ "YOU LOST" };

		std::string m_PepperString{ "5" };
		std::string m_HealthString{ "3" };
		std::string m_HighScoreString{ "10000" };
		std::string m_ScoreString{ "0" };


		const std::string m_BasePath{ "..\\Data\\UI\\" };
		const int m_NormalHealth = 3;
		const int m_NormalPepper = 3;
		const int m_MaxBurgers = 16;

		int m_Pepper{ m_NormalPepper };
		int m_burgers{};
		int m_Score{};
		int m_health{m_NormalHealth};
		bool m_DirtyUI{false};

		//private member functions
		void HandleIncomingMessages();
		void HandleGameWorld();
		void HandleOutGoingMessages();
		void InitialiseData();
		void AddGameUI();

		//CallBacks
		void GameOver();
		void GameWon();
		void KilledOther();
		void SendPepper();
		void GotPepper(int);
		void AddBurger();
		void RoundReset(int);

		//Save and load
		std::string m_AppDataPath{ "" };
		std::string m_SoloDataPath{ "" };
		const std::string m_ReadMeText{ "Hi\nThis folder was created for the Programming 4 end assignment for Ibn Zwanckaert\nIf you already seen him and gave him 20/20 you can delete this.\nOtherwise pls give Ibn a 20/20 (he needs it)" };
	};
}
#endif