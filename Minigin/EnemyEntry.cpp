#include "MiniginPCH.h"
#include "EnemyEntry.h"
#include "EnemyComponent.h"

dae::EnemyEntry::EnemyEntry(GameObject* owner) : Component(owner)
{
}

dae::EnemyEntry::~EnemyEntry()
{
}

void dae::EnemyEntry::Update(float deltaTime)
{
	int sign = -(int)glm::sign(m_Original - m_Dest);
	GetOwner()->Move(glm::vec3(m_Speed * deltaTime * sign, 0, 0));

	float distancefromorigal{ glm::abs(GetOwner()->GetPosition().GetPosition().x - m_Original)};
	float destfromorigal{ glm::abs(m_Dest - m_Original)};

	if (distancefromorigal > destfromorigal)
	{
		EnemyComponent* e = static_cast<EnemyComponent*>( GetOwner()->AddComponent<EnemyComponent>());
		e->Init(m_Type);
		GetOwner()->RemoveComponent<EnemyEntry>();
	}
}
