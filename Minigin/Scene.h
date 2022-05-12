#pragma once
#include "GameObject.h"

namespace dae
{
	class Collider;
	class Component;
	class GameManager;
	class Scene
	{
	public:
		GameObject* Add(GameObject*);
		void AddGameManager(GameManager* object) { m_gameManager = object; }
		GameManager* GetGameManager() { return m_gameManager; }
		void GetCollisions();
		std::vector<Collider*>* GetColliderVector() { return &m_colliders; }
		template<class T> Component* GetComponent() const
		{
			for (size_t i = 0; i < m_Objects.size(); i++)
			{
				Component* ptr = m_Objects[i]->FindComponent<T>();
				if (ptr != nullptr)
					return ptr;
			}
			return nullptr;
		}
		void AddCollider(Collider* coll) { m_colliders.push_back(coll); }
		void RemoveCollider(Collider* coll) { m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), coll), m_colliders.end()); }
		void RemoveGameObject(GameObject* go) { m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), go), m_Objects.end()); }

		void Update(float);
		void Render() const;
		void Start();

		Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		void DeleteGameObject(GameObject*);

		std::string m_Name;
		std::vector<GameObject*> m_Objects{};
		std::vector<Collider*> m_colliders{};
		dae::GameManager* m_gameManager{nullptr};

		static unsigned int m_IdCounter; 
	};

}
