#include "MiniginPCH.h"
#include "Component.h"

dae::Component::Component(GameObject* Owner)
	: m_Owner{Owner}
{}

dae::GameObject* dae::Component::GetOwner() const
{
	return m_Owner;
}


