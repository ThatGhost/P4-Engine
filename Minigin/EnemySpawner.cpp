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
			SpawnEnemy();
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
	if (pos.x < 0)
		rendercomp->SetRow(2);
	else
		rendercomp->SetRow(1);

	Collider* collider = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider->Setlookers(true,true);
	collider->SetPosition(glm::vec3(9,6,0));
	collider->SetDimensions(glm::vec2(14,20));
	collider->SetTag("ENEMY");
	SceneManager::GetInstance().AddCollider(collider);

	Collider* collider2 = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider2->Setlookers(false,true);
	collider2->SetPosition(glm::vec3(13,30,0));
	collider2->SetDimensions(glm::vec2(6,4));
	collider2->SetTag("BOT");
	SceneManager::GetInstance().AddCollider(collider2);

	Collider* collider3 = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider3->Setlookers(false,true);
	collider3->SetPosition(glm::vec3(13,5,0));
	collider3->SetDimensions(glm::vec2(6,4));
	collider3->SetTag("TOP");
	SceneManager::GetInstance().AddCollider(collider3);

	Collider* collider4 = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider4->Setlookers(false,true);
	collider4->SetPosition(glm::vec3(0,30,0));
	collider4->SetDimensions(glm::vec2(6,4));
	collider4->SetTag("LEFT");
	SceneManager::GetInstance().AddCollider(collider4);

	Collider* collider5 = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider5->Setlookers(false,true);
	collider5->SetPosition(glm::vec3(26,30,0));
	collider5->SetDimensions(glm::vec2(6,4));
	collider5->SetTag("RIGHT");
	SceneManager::GetInstance().AddCollider(collider5);

	m_AmountOfEnemys++;
}

void dae::EnemySpawner::RemoveEnemy()
{
	m_AmountOfEnemys--;
}

void dae::EnemySpawner::Restart()
{
	m_AmountOfEnemys = 0;
	for (size_t i{}; i < GetOwner()->GetChildCount(); i++)
	{
		GetOwner()->GetChildAt(i)->Destroy();
	}
	SpawnEnemy(0);
	SpawnEnemy(1);
}
