#include "MiniginPCH.h"
#include "EnemySpawner.h"
#include "EnemyEntry.h"
#include "RenderComponent.h"
#include "Collider.h"
#include "SceneManager.h"

dae::EnemySpawner::EnemySpawner(GameObject* owner) : Component(owner)
{
}

dae::EnemySpawner::~EnemySpawner()
{

}

void dae::EnemySpawner::Update(float deltaTime)
{
	m_Timer += deltaTime;
	if (m_Timer > m_SecUntilSpawn)
	{
		if (m_AmountOfEnemys < m_MaxEnemys)
		{
			//SpawnEnemy();
		}
		m_Timer -= m_SecUntilSpawn;
	}
}

void dae::EnemySpawner::Start()
{
	for (size_t i = 0; i < GetOwner()->GetChildCount(); i++)
	{
		m_SpawnPoints.push_back(GetOwner()->GetChildAt(i)->GetPosition().GetPosition());
	}
	SpawnEnemy(0);
	SpawnEnemy(1);
}

void dae::EnemySpawner::SpawnEnemy(int idx)
{
	GameObject* owner{ GetOwner() };
	GameObject* enemy = new GameObject();

	glm::vec3 pos{};
	if (idx == -1)
	{
		pos = m_SpawnPoints[rand() % m_SpawnPoints.size()];
	}
	else
	{
		pos = m_SpawnPoints[idx];
	}

	//general
	enemy->SetPosition(pos);
	enemy->SetParent(owner);
	enemy->SetStatic(false);

	//components
	EnemyEntry* entry = static_cast<EnemyEntry*>(enemy->AddComponent<EnemyEntry>());
	if (pos.x < 0)
		entry->Init(40);
	else
		entry->Init(424);

	RenderComponent* rendercomp = static_cast<RenderComponent*>(enemy->AddComponent<RenderComponent>());
	rendercomp->SetImage("Bean.png",true,1);
	rendercomp->SetAnimationDetails(4,0.2f);

	Collider* collider = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider->Setlookers(true,true);
	collider->SetPosition(glm::vec3(0,0,0));
	collider->SetDimensions(glm::vec2(32,32));
	collider->SetTag("ENEMY");
	SceneManager::GetInstance().AddColliderToMain(collider);

	m_AmountOfEnemys++;
}

void dae::EnemySpawner::RemoveEnemy()
{
	m_AmountOfEnemys--;
}