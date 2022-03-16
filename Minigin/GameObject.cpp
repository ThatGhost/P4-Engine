#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

//dae::GameObject::GameObject(std::string name) : m_Name(name)
//{
//
//}

void dae::GameObject::Update(float deltaTime)
{
	for (auto& comp : m_Components)
	{
		comp.get()->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& renderComp : m_Components)
	{
		renderComp->Render();
	}
}

template <typename T> 
T* dae::GameObject::AddComponent()
{
	m_Components.push_back(std::make_unique<T>(&this));
	return m_Components[m_Components.size()-1].get();
}

template <typename T> 
T* dae::GameObject::GetComponent() const
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

template <typename T> 
bool dae::GameObject::RemoveComponent()
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

dae::GameObject* dae::GameObject::SetParent()
{
	m_Parent = std::make_shared<GameObject>();
	return m_Parent.get();
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent.get();
}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	return m_Children[index].get();
}

void dae::GameObject::RemoveChild(int index)
{
	m_Children.erase(std::next(m_Children.begin(), index));
}

dae::GameObject* dae::GameObject::AddChild()
{
	m_Children.push_back(std::make_unique<GameObject>());
	return m_Children[m_Children.size()-1].get();
}