#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class GameManager;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene* CreateScene(const std::string& name);
		dae::Scene* GetActiveScene() { return m_Scenes[m_ActiveScene].get(); }

		void Update(float);
		void Render();
		void KillMainScene();
		void KillAllScenes();
		void SwitchScene(const std::string& scenename);
		void AddCollider(Collider*);
		void RemoveCollider(Collider*);
		GameManager* GetGameManager();
		void SetGameManager(GameManager*);
		void RemoveGameObject(GameObject* object) {	m_Scenes[m_ActiveScene].get()->RemoveGameObject(object); }

		template<class T> Component* FindComponent() const
		{
			for (size_t i = 0; i < m_Scenes.size(); i++)
			{
				Component* comp = m_Scenes[i].get()->GetComponent<T>();
				if (comp != nullptr)
				{
					return comp;
				}
			}
			return nullptr;
		}
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager() = default;

		std::vector<std::unique_ptr<dae::Scene>> m_Scenes;
		int m_ActiveScene{1};
	};
}
