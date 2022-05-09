#include "MiniginPCH.h"
#include "GameManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "UIManager.h"

dae::GameManager::GameManager(GameObject* owner) : Component(owner)
{
	EventManager::AddEvent("DIE",std::bind(&dae::GameManager::OnDie,this,m_Argument));
	EventManager::AddEvent("WIN",std::bind(&dae::GameManager::OnWin,this,m_Argument));
}

dae::GameManager::~GameManager()
{

}

void dae::GameManager::Restart()
{
	SceneManager::GetInstance().SwitchScene("Level1.json");
}

void dae::GameManager::GameOver()
{
	UIManager::GetInstance().ClearUI();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
	if (m_score > m_HighScore)
	{
		m_HighScore = m_score;
	}
}

void dae::GameManager::OnDie(byte*)
{
	m_Health--;
	if (m_Health == 0)
		GameOver();
	else
		Restart();
}

void dae::GameManager::OnWin(byte*)
{
	UIManager::GetInstance().ClearUI();
	SceneManager::GetInstance().SwitchScene("MainMenu.json");
}
