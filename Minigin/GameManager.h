#pragma once
#include "Component.h"

namespace dae
{
	using byte = unsigned char;
	class PlayerController;
	class GameManager : public Component
	{
	public:
		GameManager(GameObject* owner);
		~GameManager() override;

	private:
		void Restart();
		void GameOver();
		void OnDie(byte*);
		void OnWin(byte*);

		PlayerController* m_playerController = nullptr;

		byte* m_Argument = nullptr;
		const int m_NormalHealth = 5;
		const int m_NormalSalt = 3;
		int m_HighScore = 0;

		int m_score = 0;
		int m_Health = m_NormalHealth;
		int m_Salt = m_NormalSalt;
	};
}