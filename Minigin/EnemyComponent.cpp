#include "MiniginPCH.h"
#include "EnemyComponent.h"
#include "EnemySpawner.h"
#include "RenderComponent.h"
#include "Collider.h"
#include "EventManager.h"

dae::EnemyComponent::EnemyComponent(GameObject* owner) : Component(owner)
{
	EventManager::AddEvent("0BUTTON_Y",std::bind(&dae::EnemyComponent::OnY,this,m_Argument));

	m_spawner = static_cast<EnemySpawner*>(GetOwner()->GetParent()->GetComponent<EnemySpawner>());
	m_renderer = static_cast<RenderComponent*>(GetOwner()->GetComponent<RenderComponent>());
}

dae::EnemyComponent::~EnemyComponent()
{

}

void dae::EnemyComponent::Update(float)
{

}

void dae::EnemyComponent::Die()
{
	if (!m_isDead)
	{
		EventManager::RemoveEvent("0BUTTON_Y");
		m_spawner->RemoveEnemy();
		GetOwner()->Destroy();
		m_isDead = true;
	}
}

void dae::EnemyComponent::Salt()
{

}

void dae::EnemyComponent::OnY(byte*)
{
	Die();
}

void dae::EnemyComponent::OnCollisionEnter(Collider*, Collider*)
{

}