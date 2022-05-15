#include "MiniginPCH.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "Scene.h"
#include "GameManager.h"
#include "SceneConstructor.h"

void dae::SceneManager::Update(float deltaTime)
{
	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		m_Scenes[i]->Update(deltaTime);
	}
	if (m_switchScene)
	{
		ChangeScene(m_nextSceneName);
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
	m_switchScene = true;
	m_nextSceneName = scenename;
}

void dae::SceneManager::ConstructEmptyScene(const std::string& scenename)
{
	CreateScene(scenename);
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

void dae::SceneManager::ChangeScene(const std::string& name)
{
	SceneConstructor::ConstructScene(name);
	m_Scenes[m_ActiveScene].get()->Start();
	m_switchScene = false;
}
