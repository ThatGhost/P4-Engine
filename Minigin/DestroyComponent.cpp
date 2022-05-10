#include "MiniginPCH.h"
#include "DestroyComponent.h"

dae::DestroyComponent::DestroyComponent(GameObject* owner) : Component(owner)
{
}

void dae::DestroyComponent::Update(float deltaTime)
{
	if (m_Run)
	{
		if (m_Timer >= m_Time)
		{
			GetOwner()->Destroy();
		}
		else m_Timer += deltaTime;
	}
}
