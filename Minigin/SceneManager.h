#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void SetActiveScene(int index) { m_ActiveScene = index; }
		std::shared_ptr<Scene> GetActiveScene() { return m_Scenes[m_ActiveScene]; }

		void Update(float);
		void Render();
		void ClearScenes();
		void SwitchScene(const std::string& scenename);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		~SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_ActiveScene{0};
	};
}
