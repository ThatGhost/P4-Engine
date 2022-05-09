#pragma once
#include "GameObject.h"

namespace dae
{
	class Collider;
	class Component;
	class Scene
	{
	public:
		void Add(GameObject* object);
		void GetCollisions();
		std::vector<Collider*>* GetColliderVector() { return &m_colliders; }
		template<class T> Component* GetComponent() const
		{
			for (auto g : m_Objects)
			{
				Component* ptr = g->FindComponent<T>();
				if (ptr != nullptr)
					return ptr;
			}
			return nullptr;
		}
		void AddCollider(Collider* coll) { m_colliders.push_back(coll); }

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

		static unsigned int m_IdCounter; 
	};

}
