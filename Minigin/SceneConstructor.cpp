#include "MiniginPCH.h"
#include "SceneConstructor.h"
#include <fstream>
#include <map>
#include "json.hpp"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"

#include "Collider.h"
#include "TestComponent.h"
#include "RenderComponent.h"

using json = nlohmann::json;

std::string dae::SceneConstructor::TrimJson(const std::string& string)
{
	return string.substr(0, string.size());
}

void dae::SceneConstructor::ConstructScene(const std::string& nameScene)
{
	std::ifstream SceneFile("..\\Data\\"+nameScene, std::ifstream::binary);
	json sceneData;
	sceneData = json::parse(SceneFile);
	auto& scene = SceneManager::GetInstance().CreateScene(TrimJson(sceneData["name"]));
	std::vector<dae::Collider*>* colliders = scene.GetColliderVector();

	for (auto it = sceneData["gameObjects"].begin(); it != sceneData["gameObjects"].end(); ++it)
	{
		scene.Add(ConstructGO(it,colliders));
	}

	//add all colliders to each other 
	for (auto coll : *colliders)
	{
		for (auto colltoadd : *colliders)
		{
			if (coll != colltoadd)
			{
				if (!colltoadd->GetOwner()->IsStatic())
				{
					coll->AddCollider(colltoadd);
				}
				else if (!coll->GetOwner()->IsStatic())
				{
					coll->AddCollider(colltoadd);
				}
			}
		}
	}
}

dae::GameObject* dae::SceneConstructor::ConstructGO(const nlohmann::json::iterator& it, std::vector<dae::Collider*>* colliders)
{
	auto gameObject = new dae::GameObject();

	float x = std::stof(TrimJson(it.value()["x"]));
	float y = std::stof(TrimJson(it.value()["y"]));
	float z = std::stof(TrimJson(it.value()["z"]));

	gameObject->Move(glm::vec3(x, y, z));
	gameObject->SetStatic(TrimJson(it.value()["static"]) == "true");

	std::cout << "construct gameobject -> Name : " << TrimJson(it.value()["name"]) << std::endl;

	for (auto compIt = it.value()["components"].begin(); compIt != it.value()["components"].end(); ++compIt)
	{
		std::string NameComponent{ TrimJson(compIt.value()["name"]) };
		Components c = StringToType.at(NameComponent);
		switch (c)
		{
		case Components::collider:
		{
			dae::Collider* coll = static_cast<dae::Collider*>(gameObject->AddComponent<dae::Collider>());

			std::string xs = TrimJson(compIt.value()["x"]);
			float cx = (xs == "0") ? std::stof(xs) : x;
			std::string ys = TrimJson(compIt.value()["y"]);
			float cy = (ys == "0") ? std::stof(ys) : y;
			coll->SetPosition(glm::vec3(cx, cy, 0));

			int cw = std::stoi(TrimJson(compIt.value()["width"]));
			int ch = std::stoi(TrimJson(compIt.value()["height"]));
			coll->SetDimensions(glm::vec2(cw, ch));

			if (TrimJson(compIt.value()["enter"]) != "NULL")
				coll->PassEventEnter(TrimJson(compIt.value()["enter"]));
			if (TrimJson(compIt.value()["exit"]) != "NULL")
				coll->PassEventEnter(TrimJson(compIt.value()["exit"]));
			colliders->push_back(coll);
		}
		break;
		case Components::renderComponent:
		{
			dae::RenderComponent* render = static_cast<dae::RenderComponent*>(gameObject->AddComponent<dae::RenderComponent>());
			bool animated = TrimJson(compIt.value()["animated"]) == "true";
			render->SetImage(TrimJson(compIt.value()["image"]), animated);
		}
		break;
		case Components::testComponent:
		{
			dae::TestComponent* test = static_cast<dae::TestComponent*>(gameObject->AddComponent<dae::TestComponent>());
			
			test;
		}
		break;
		default:
			std::cout << "NO KNOWN COMPONENT TYPE \n";
			break;
		}
	}

	//children
	for (auto childIt = it.value()["children"].begin(); childIt != it.value()["children"].end(); ++childIt)
	{
		ConstructGO(childIt,colliders)->SetParent(gameObject);
	}

	return gameObject;
}