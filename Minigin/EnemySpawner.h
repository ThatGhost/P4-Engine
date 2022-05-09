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
		virtual void Start() override;

		void RemoveEnemy();
	private:

		void SpawnEnemy(int idx = -1);
		const float m_SecUntilSpawn{ 3 };
		const int m_MaxEnemys{ 3 };

		std::vector<glm::vec3> m_SpawnPoints{};

		float m_Timer{};
		int m_AmountOfEnemys{};
		bool m_initialPhase{true};
	};
}

