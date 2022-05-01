#pragma once
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;
	class Component;
	class RenderComponent;

	class GameObject final 
	{
	public:
		void Update(float);
		void Render() const;

		void Move(const glm::vec3& pos);
		dae::Transform GetPosition();
		void SetPosition(const glm::vec3& pos) { m_RelativeTransform.SetPosition(pos.x, pos.y, pos.z); m_PositionDirty = true; }

		template <typename T> dae::Component* AddComponent()
		{
			m_Components.push_back(std::make_unique<T>(this));
			return m_Components[m_Components.size()-1].get();
		}
		template <typename T> dae::Component* GetComponent() const
		{
			for (auto comp : m_Components)
			{
				if (typeid(*comp.get()) == typeid(T))
				{
					return comp.get();
				}
			}
			return NULL;
		}
		template <typename T> bool RemoveComponent()
		{
			for (auto comp : m_Components)
			{
				if (typeid(*comp.get()) == typeid(T))
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

		bool IsStatic() const { return m_Static; }
		void SetStatic(bool isStatic) { m_Static = isStatic; }

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	protected:
		void RemoveChild(int index);
		void RemoveChild(GameObject*);
		GameObject* AddChild(GameObject* child);
		void SetDirty(bool dirty = true) { m_PositionDirty = dirty; }

	private:
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<GameObject*> m_Children{};
		GameObject* m_Parent{nullptr};

		bool m_PositionDirty{false};
		dae::Transform m_RelativeTransform{};
		dae::Transform m_TrueTransform{};
		bool m_Static{ false };

		dae::Transform CalculatePosition();
	};
}
