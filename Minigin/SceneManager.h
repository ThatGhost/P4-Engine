#pragma once
#include "Singleton.h"
#include "Scene.h"

namespace dae
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		std::shared_ptr<dae::Scene> GetActiveScene() { return m_Scenes[m_ActiveScene]; }

		void Update(float);
		void Render();
		void KillMainScene();
		void KillAllScenes();
		void SwitchScene(const std::string& scenename);
		void AddColliderToMain(Collider*);

		template<class T> Component* FindComponent() const
		{
			for (auto scene : m_Scenes)
			{
				Component* comp = scene.get()->GetComponent<T>();
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

		std::vector<std::shared_ptr<dae::Scene>> m_Scenes;
		int m_ActiveScene{1};
	};
}
