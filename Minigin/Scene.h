#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Collider;
	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(GameObject* object);
		void GetCollisions();
		std::vector<Collider*>* GetColliderVector() { return &m_colliders; }

		void Update(float);
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);
		void DeleteGameObject(GameObject*);

		std::string m_Name;
		std::vector<GameObject*> m_Objects{};
		std::vector<Collider*> m_colliders{};

		static unsigned int m_IdCounter; 
	};

}
