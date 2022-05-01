#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "Collider.h"

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
		object->Update(deltaTime);
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
	//for (const auto& object : m_Objects)
	//{
	//	object->Render();
	//}
}

void Scene::GetCollisions()
{
	for (auto coll : m_colliders)
	{
		coll->CheckCollisions();
	}
}

