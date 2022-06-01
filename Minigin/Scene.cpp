#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Collider.h"
#include <algorithm>

dae::Scene::Scene(const std::string& name) : m_Name(name) {}

dae::Scene::~Scene()
{
	for (auto go : m_Objects)
	{
		delete go;
	}
}

dae::GameObject* dae::Scene::Add(GameObject* obj)
{
	m_Objects.push_back(obj);
	return obj;
}

void dae::Scene::Update(float deltaTime)
{
	bool deleted = false;

	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] != nullptr)
		{
			if (!m_Objects[i]->IsMarkedForDeletion())
			{
				m_Objects[i]->Update(deltaTime);
			}
			else if (!deleted)
			{
				DeleteGameObject(m_Objects[i]);
				i--;
				deleted = true;
			}
		}
	}
}

void dae::Scene::Render() const
{
	std::vector<dae::GameObject*> toRender{ };
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		toRender.push_back(m_Objects[i]);
	}

	int layer{0};
	while (toRender.size() > 0)
	{
		for (size_t i = 0; i < toRender.size(); i++)
		{
			if (toRender[i]->GetPosition().GetPosition().z == layer)
			{
				toRender[i]->Render();
				toRender.erase(toRender.begin() + i);
				--i;
			}
		}
		layer++;
	}
}

void dae::Scene::Start()
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Start();
	}
}

void dae::Scene::GetCollisions()
{
	for (auto coll : m_colliders)
	{
		coll->CheckCollisions(&m_colliders);
	}
}

void dae::Scene::DeleteGameObject(GameObject* obj)
{
	int idx = int(m_Objects.begin() - std::find(m_Objects.begin(), m_Objects.end(), obj));
	delete obj;
	obj = nullptr;
	m_Objects.erase(m_Objects.begin() + idx);
}