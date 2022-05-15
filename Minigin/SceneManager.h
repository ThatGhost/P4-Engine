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
		dae::Scene* GetScene(int i) { return m_Scenes[i].get(); }
		GameManager* GetGameManager();

		void Update(float);
		void Render();
		void KillMainScene();
		void KillAllScenes();
		void SwitchScene(const std::string& scenename);
		void ConstructEmptyScene(const std::string& scenename);

		void AddCollider(Collider*);
		void RemoveCollider(Collider*);
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

		void ChangeScene(const std::string& name);

		bool m_switchScene{false};
		std::string m_nextSceneName{};

		std::vector<std::unique_ptr<dae::Scene>> m_Scenes;
		int m_ActiveScene{1};
	};
}
