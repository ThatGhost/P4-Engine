#include "MiniginPCH.h"
#include "BurgerComponent.h"

dae::BurgerComponent::BurgerComponent(GameObject* Owner) : Component(Owner)
{
	static int id = 0;
	m_BurgerId = id++;
}

dae::BurgerComponent::~BurgerComponent()
{

}

void dae::BurgerComponent::Update(float deltaTime)
{
	if (m_isFalling)
	{
		GetOwner()->Move(glm::vec3(0, m_FallSpeed * deltaTime,0));
	}
}