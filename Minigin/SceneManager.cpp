#include "MiniginPCH.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "EventManager.h"
#include "SceneConstructor.h"

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_Scenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
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

void dae::SceneManager::AddColliderToMain(Collider* coll)
{
	m_Scenes[m_ActiveScene].get()->AddCollider(coll);
}
