#pragma once
#include "Component.h"

namespace dae
{
	class EnemySpawner : public Component
	{
	public:
		EnemySpawner(GameObject*);
		virtual ~EnemySpawner() override;
		virtual void Update(float) override;

		void RemoveEnemy();
	private:

		void SpawnEnemy();
		const float m_SecUntilSpawn{ 3 };
		const int m_MaxEnemys{ 2 };

		std::vector<dae::Collider*> m_Empty{};

		float m_Timer{};
		int m_AmountOfEnemys{};
		bool m_initialPhase{true};
	};
}

