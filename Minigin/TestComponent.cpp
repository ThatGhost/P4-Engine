#include "MiniginPCH.h"
#include "TestComponent.h"
#include "EventManager.h"
#include "InputManager.h"
#include "UIManager.h"
#include "ResourceManager.h"

dae::TestComponent::TestComponent(GameObject* Owner) : Component(Owner)
{
	EventManager::AddEvent("0BUTTONA", std::bind(&dae::TestComponent::Test, this, m_Argument));
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonA, "BUTTONA");
	//m_UIImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\Hello.png");
	UIManager::GetInstance().AddTextElement(&m_Sting,25,glm::vec2(100,100));
}

dae::TestComponent::~TestComponent()
{
}

void dae::TestComponent::Test(byte*)
{
	m_Sting = "hahahhahaha";
	UIManager::GetInstance().UpdateUI();
	//m_UIImage = ResourceManager::GetInstance().LoadTexture("..\\Data\\UI\\Hello2.png");
}