#include "MiniginPCH.h"
#include "TestComponent.h"
#include "EventManager.h"
#include "InputManager.h"

dae::TestComponent::TestComponent(GameObject* Owner) : Component(Owner)
{
	EventManager::AddEvent("TESTCOLLIDER1", std::bind(&dae::TestComponent::Test,this,m_Test));
	EventManager::AddEvent("0XButton", std::bind(&dae::TestComponent::OnInputX,this,m_Test));

	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonA,"XButton");
	std::cout << "Added input and event XButton";
}

dae::TestComponent::~TestComponent()
{

}

void dae::TestComponent::Test(float)
{
	std::cout << "Test Worked!!!!" << std::endl;
}

void dae::TestComponent::OnInputX(float)
{
	std::cout << "X Button pressed" << std::endl;
}