#pragma once
#include "Component.h"
#include "EnemyComponent.h"
namespace dae
{
	class EnemySpawner : public Component
	{
	public:
		EnemySpawner(GameObject*);
		virtual ~EnemySpawner() override;
		virtual void Update(float) override;
		virtual void Start() override;

		void RemoveEnemy(EnemyType);
		void Restart();
		void ClearEnemys();
		void SetSpawning(bool spawning = true) { m_Spawning = spawning; }
	private:

		void SpawnEnemy(int idx = -1);
		const float m_SecUntilSpawn{ 3 };
		const int m_MaxEnemys{ 6 };

		std::vector<glm::vec3> m_SpawnPoints{};

		float m_Timer{};
		int m_AmountOfEnemys{};
		bool m_EggOnScreen{false};
		bool m_Spawning{ true };
	};
}

