#include "MiniginPCH.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Scene.h"
#include "GameManager.h"
#include "SceneConstructor.h"

#include "Collider.h"

void dae::SceneManager::Update(float deltaTime)
{
	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		m_Scenes[i]->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		m_Scenes[i]->Render();
	}
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name)
{
	m_Scenes.push_back(std::make_unique<Scene>(name));
	return m_Scenes[m_Scenes.size() - 1].get();
}

void dae::SceneManager::KillMainScene()
{
	if(m_Scenes.size() > 1)
		m_Scenes.erase(m_Scenes.end()-1);
}

void dae::SceneManager::KillAllScenes()
{
	m_Scenes.clear();
}

void dae::SceneManager::SwitchScene(const std::string& scenename)
{
	SceneConstructor::ConstructScene(scenename);
	m_Scenes[m_ActiveScene].get()->Start();
}

void dae::SceneManager::AddCollider(Collider* coll)
{
	m_Scenes[m_ActiveScene].get()->AddCollider(coll);
}

void dae::SceneManager::RemoveCollider(Collider* coll)
{
	m_Scenes[m_ActiveScene].get()->RemoveCollider(coll);
}

dae::GameManager* dae::SceneManager::GetGameManager()
{
	return m_Scenes[m_ActiveScene].get()->GetGameManager();
}

void dae::SceneManager::SetGameManager(GameManager* gm)
{
	m_Scenes[m_ActiveScene].get()->AddGameManager(gm);
}
