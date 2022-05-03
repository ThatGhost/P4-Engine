#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <memory>


dae::GameObject::~GameObject()
{
	for (auto& comp : m_Children)
	{
		delete comp;
	}
}

void dae::GameObject::Update(float deltaTime)
{
	for (auto& comp : m_Components)
	{
		comp->Update(deltaTime);
	}

	for (auto child : m_Children)
	{
		child->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& renderComp : m_Components)
	{
		renderComp->Render();
	}

	for (auto child : m_Children)
	{
		child->Render();
	}
}

void dae::GameObject::Move(const glm::vec3& pos)
{
	if (m_Static) return;

	glm::vec3 current = m_RelativeTransform.GetPosition();
	m_RelativeTransform.SetPosition(current.x + pos.x,current.y + pos.y, current.z + pos.z);

	m_PositionDirty = true;
	for (auto child : m_Children)
	{
		child->SetDirty();
	}
}

dae::Transform dae::GameObject::GetPosition()
{
	if (m_PositionDirty)
	{
		m_TrueTransform = CalculatePosition();
		m_PositionDirty = false;
	}
	
	return m_TrueTransform;
}

dae::Transform dae::GameObject::CalculatePosition()
{
	if (m_Parent != nullptr)
	{
		glm::vec3 parentTrueTransform = m_Parent->GetPosition().GetPosition();
		glm::vec3 relativeTransform = m_RelativeTransform.GetPosition();
		m_TrueTransform.SetPosition(relativeTransform.x + parentTrueTransform.x, 
									relativeTransform.y + parentTrueTransform.y, 
									relativeTransform.z + parentTrueTransform.z);
		return m_TrueTransform;
	}
	return m_RelativeTransform;
}

dae::GameObject* dae::GameObject::SetParent(GameObject* parent)
{
	if(m_Parent!= nullptr)
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
	if(index < (int)m_Children.size())
		m_Children.erase(std::next(m_Children.begin(), index));
}

void dae::GameObject::RemoveChild(GameObject* obj)
{
	if (m_Children.size() > 0)
	{
		for (size_t i = 0; i < m_Children.size(); i++) {
			if (m_Children[i] == obj) {
				m_Children.erase(m_Children.begin() + i);
				break;
			}
		}
	}
}

dae::GameObject* dae::GameObject::AddChild(GameObject* child)
{
	m_Children.push_back(child);
	return m_Children[m_Children.size() - 1];
}