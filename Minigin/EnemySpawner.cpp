#include "MiniginPCH.h"
#include "EnemySpawner.h"
#include "EnemyComponent.h"
#include "RenderComponent.h"
#include "Collider.h"

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

void dae::EnemySpawner::SpawnEnemy()
{
	GameObject* owner{ GetOwner() };
	GameObject* enemy = new GameObject();

	//general
	enemy->SetPosition(glm::vec3((50 + rand()%40), 100, 3));
	enemy->SetParent(owner);
	enemy->SetStatic(false);

	//components
	enemy->AddComponent<EnemyComponent>(); //<-- how you add components

	RenderComponent* rendercomp = static_cast<RenderComponent*>(enemy->AddComponent<RenderComponent>());
	rendercomp->SetImage("Bean.png");

	Collider* collider = static_cast<Collider*>(enemy->AddComponent<Collider>());
	collider->AddLookId(0); //player
	collider->AddLookId(1); //stairs
	collider->AddLookId(2); //platform
	collider->AddLookId(4); //burger
	collider->SetId(5); //my id of collider
	collider->SetPosition(glm::vec3(0,0,0));
	collider->SetDimensions(glm::vec2(32,32));
	collider->SetTag("ENEMY");

	m_AmountOfEnemys++;
}

void dae::EnemySpawner::RemoveEnemy()
{
	m_AmountOfEnemys--;
}