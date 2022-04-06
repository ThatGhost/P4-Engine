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
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Scene::GetCollisions()
{
	for (auto coll : m_colliders)
	{
		coll->CheckCollisions();
	}
}

