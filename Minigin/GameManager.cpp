#include "MiniginPCH.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "UIManager.h"

dae::GameManager::GameManager(GameObject* owner) : Component(owner)
{
	EventManager::AddEvent("0BUTTON_B",std::bind(&GameManager::GoBack,this));
}

void dae::GameManager::GoBack()
{
	UIManager::GetInstance().ClearUI();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}
