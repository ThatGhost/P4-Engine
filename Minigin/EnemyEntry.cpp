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
	int sign = -(int)glm::sign(m_original - m_dest);
	GetOwner()->Move(glm::vec3(m_Speed * deltaTime * sign, 0, 0));

	float distancefromorigal{ glm::abs(GetOwner()->GetPosition().GetPosition().x - m_original)};
	float destfromorigal{ glm::abs(m_dest - m_original)};

	if (distancefromorigal > destfromorigal)
	{
		GetOwner()->AddComponent<EnemyComponent>();
		GetOwner()->RemoveComponent<EnemyEntry>();
	}
}
