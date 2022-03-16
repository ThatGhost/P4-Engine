#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::~GameObject()
{
	for (auto child : m_Children)
	{
		delete child;
	}
}

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

dae::GameObject* dae::GameObject::SetParent(GameObject* parent)
{
	m_Parent->RemoveChild(this);
	m_Parent = parent;
	m_Parent->AddChild(this);
	return m_Parent;
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	return m_Children[index];
}

void dae::GameObject::RemoveChild(int index)
{
	m_Children.erase(std::next(m_Children.begin(), index));
}

void dae::GameObject::RemoveChild(GameObject* obj)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), obj), m_Children.end());
}

dae::GameObject* dae::GameObject::AddChild(GameObject* child)
{
	m_Children.push_back(child);
	return child;
}