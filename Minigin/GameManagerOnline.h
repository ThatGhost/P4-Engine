#pragma once
#include "GameManager.h"
#include "olcPGEX_Network.h"
#include "MMO_Common.h"
#include <unordered_map>

namespace dae
{
	class OnlinePlayerController;
	class OnlineEnemyController;
	class GameManagerOnline : public GameManager, olc::net::client_interface<GameMsg>
	{
	public:
		GameManagerOnline(GameObject*);
		~GameManagerOnline() override;

		void Update(float) override;
		void Start() override;

		void SetPlayer(OnlinePlayerController* p) { m_playerController = p; std::cout << "added player\n"; }
		void SetEnemy(OnlineEnemyController* p) { m_EnemyController = p;}
		
	private:
		std::unordered_map<uint32_t, sPlayerDescription> m_mapObjects;
		uint32_t m_nPlayerID = 0;
		sPlayerDescription m_descPlayer;

		OnlinePlayerController* m_playerController = nullptr;
		OnlineEnemyController* m_EnemyController = nullptr;

		bool m_bWaitingForConnection = true;
		bool m_IsGoodPlayer;

		void HandleIncomingMessages();
		void HandleGameWorld();
		void HandleOutGoingMessages();

		void GameOver();
		void GameWon();
		void Salt();

		int m_burgers{};
		const int m_MaxBurgers = 16;
		void AddBurger();
	};
}
