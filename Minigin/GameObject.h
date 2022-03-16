#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class Component;
	class RenderComponent;

	class GameObject final : public SceneObject
	{
	public:
		void Update(float) override;
		void Render() const override;

		template <typename T> dae::Component* AddComponent()
		{
			m_Components.push_back(std::make_unique<T>(this));
			return m_Components[m_Components.size() - 1].get();
		}
		template <typename T> dae::Component* GetComponent() const
		{
			for (auto comp : m_Components)
			{
				if (typeid(comp) == typeid(T))
				{
					return comp;
				}
			}
			return NULL;
		}
		template <typename T> bool RemoveComponent()
		{
			for (auto comp : m_Components)
			{
				if (typeid(comp) == typeid(T))
				{
					m_Components.erase(comp);
					return true;
				}
			}
			return false;
		}

		GameObject* SetParent(GameObject*);
		GameObject* GetParent() const;

		size_t GetChildCount() const;
		GameObject* GetChildAt(int index) const;

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject & other) = delete;
		GameObject(GameObject && other) = delete;
		GameObject& operator=(const GameObject & other) = delete;
		GameObject& operator=(GameObject && other) = delete;

	protected:
		void RemoveChild(int index);
		void RemoveChild(GameObject*);
		GameObject* AddChild(GameObject* child);

	private:
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<GameObject*> m_Children{};
		GameObject* m_Parent{};
		dae::Transform m_Transform{};
	};
}
