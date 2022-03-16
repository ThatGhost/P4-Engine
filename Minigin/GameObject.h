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

	// todo: this should become final.
	class GameObject final : public SceneObject
	{
	public:
		void Update(float) override;
		void Render() const override;

		template <typename T> T* AddComponent();
		template <typename T> T* AddRenderComponent();
		template <typename T> T* GetComponent() const;
		template <typename T> bool RemoveComponent();

		GameObject* SetParent();
		GameObject* GetParent() const;

		size_t GetChildCount() const;
		GameObject* GetChildAt(int index) const;
		void RemoveChild(int index);
		GameObject* AddChild();

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::unique_ptr<Component>> m_Components{};
		std::vector<std::unique_ptr<GameObject>> m_Children{};
		std::shared_ptr<GameObject> m_Parent{};
		std::string m_Name{};
	};
}
