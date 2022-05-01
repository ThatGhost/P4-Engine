#include "MiniginPCH.h"
#include "TestComponent.h"
#include "EventManager.h"
#include "InputManager.h"

dae::TestComponent::TestComponent(GameObject* Owner) : Component(Owner)
{
	EventManager::AddEvent("TESTCOLLIDER1", std::bind(&dae::TestComponent::Test,this,m_Test));
}

dae::TestComponent::~TestComponent()
{

}

void dae::TestComponent::Test(float)
{
	std::cout << "TEST (TestComponent 18):: Test passed" << std::endl;
}