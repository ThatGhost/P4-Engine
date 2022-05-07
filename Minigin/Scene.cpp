#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Collider.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene()
{
	for (auto go : m_Objects)
	{
		delete go;
	}
}

void Scene::Add(GameObject* object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_Objects)
	{
		if (!object->IsMarkedForDeletion())
			object->Update(deltaTime);
		else 
			DeleteGameObject(object);
	}
}

void Scene::Render() const
{
	std::vector<dae::GameObject*> toRender{ m_Objects };
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

void Scene::GetCollisions()
{
	for (auto coll : m_colliders)
	{
		coll->CheckCollisions();
	}
}

void dae::Scene::DeleteGameObject(GameObject* obj)
{
	if (std::find(m_Objects.begin(), m_Objects.end(), obj) == m_Objects.end()) //if vector doesnt contain pointer
	{
		obj->SetParent(nullptr);
		delete obj;
		obj = nullptr;
	}
	else
	{
		int idx = m_Objects.begin() - std::find(m_Objects.begin(), m_Objects.end(), obj);
		delete obj;
		obj = nullptr;
		m_Objects.erase(m_Objects.begin() + idx);
	}
}