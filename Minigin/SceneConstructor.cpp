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
#include "PlayerController.h"
#include "BurgerComponent.h"
#include "BurgerHolder.h"

using json = nlohmann::json;
bool m_canRecreate = false;

std::string dae::SceneConstructor::TrimJson(const std::string& string)
{
	return string.substr(0, string.size());
}

void dae::SceneConstructor::OnReCreateScene(float)
{
	std::cout << "recreate scene\n";
	if (!m_canRecreate)
		return;

	m_canRecreate = false;
	ConstructScene("JsonFileLvl1.json");
}

float g_arg;

void dae::SceneConstructor::Init()
{
	EventManager::AddEvent("0LSHOULDER", std::bind(&dae::SceneConstructor::OnReCreateScene, g_arg));
	InputManager::GetInstance().InsertCommand(dae::ControllerButton::ButtonLeftShoulder, "LSHOULDER");
	m_canRecreate = true;
}

void dae::SceneConstructor::ConstructScene(const std::string& nameScene)
{
	SceneManager::GetInstance().ClearScenes();

	//read file
	std::ifstream SceneFile("..\\Data\\Levels\\"+nameScene, std::ifstream::binary);
	json sceneData;
	sceneData = json::parse(SceneFile);
	SceneFile.close();

	//make scene
	auto& scene = SceneManager::GetInstance().CreateScene(TrimJson(sceneData["name"]));
	std::vector<dae::Collider*>* colliders = scene.GetColliderVector();

	//construct the gameobjects
	//for (auto it = sceneData["gameObjects"].begin(); it != sceneData["gameObjects"].end(); ++it)
	//{
	//	std::cout << it.value();
	//	json j{ it.value()};
	//	scene.Add(ConstructGO(j, colliders));
	//}

	for (auto it = sceneData["prefabs"].begin(); it != sceneData["prefabs"].end(); ++it)
	{
		std::ifstream prefabFile("..\\Data\\Prefabs\\" + TrimJson(it.value()["name"]), std::ifstream::binary);
		json j = json::parse(prefabFile);
		prefabFile.close();
		j["x"] = it.value()["x"];
		j["y"] = it.value()["y"];
		j["z"] = it.value()["z"];
		scene.Add(ConstructGO(j,colliders));
	}



	//add all neccesairy colliders to each other:

	for (auto coll : *colliders)
	{
		if (coll->GetLookId() != -1)
		{
			for (auto colltoadd : *colliders)
			{
				if (coll->GetOwner() != colltoadd->GetOwner() && coll->GetLookId() == colltoadd->GetId())
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

	m_canRecreate = true;
}

dae::GameObject* dae::SceneConstructor::ConstructGO(const json& it, std::vector<dae::Collider*>* colliders)
{
	auto gameObject = new dae::GameObject();

	//position
	float x = std::stof(TrimJson(it["x"]));
	float y = std::stof(TrimJson(it["y"]));
	float z = std::stof(TrimJson(it["z"]));

	gameObject->SetPosition(glm::vec3(x, y, z));

	//static
	gameObject->SetStatic(TrimJson(it["static"]) == "true");

	//std::cout << "construct gameobject -> Name : " << TrimJson(it["name"]) << " -> (" << x << ", " << y << ")" << std::endl;

	//components 
	for (auto compIt = it["components"].begin(); compIt != it["components"].end(); ++compIt)
	{
		std::string NameComponent{ TrimJson(compIt.value()["name"])};
		Components c = StringToType.at(NameComponent);

		switch (c)
		{
		case Components::collider:
		{
			dae::Collider* coll = static_cast<dae::Collider*>(gameObject->AddComponent<dae::Collider>());

			//position of collider
			std::string xs = TrimJson(compIt.value()["x"]);
			float cx = std::stof(xs);
			std::string ys = TrimJson(compIt.value()["y"]);
			float cy = std::stof(ys);
			coll->SetPosition(glm::vec3(cx, cy, 0));

			//dimensions of collider
			int cw = std::stoi(TrimJson(compIt.value()["width"]));
			int ch = std::stoi(TrimJson(compIt.value()["height"]));
			coll->SetDimensions(glm::vec2(cw, ch));

			//triggers of collider
			std::string event{ TrimJson(compIt.value()["event"]) };
			if (event != "NULL")
				coll->PassEvent(event);

			bool empty{ TrimJson(compIt.value()["lookAt"])  == "NULL"};
			int lookAtValue{empty ? -1 : std::stoi(TrimJson(compIt.value()["lookAt"])) };
			coll->SetId(std::stoi(TrimJson(compIt.value()["id"])), lookAtValue);
			//put collider in the big list
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
		case Components::playerController:
		{
			dae::PlayerController* playerController = static_cast<dae::PlayerController*>(gameObject->AddComponent<dae::PlayerController>());
			playerController->Init(std::stoi(TrimJson(compIt.value()["player"])));
		}
		break;
		case Components::burgerHolder:
		{
			dae::BurgerHolder* burgerHolder = static_cast<dae::BurgerHolder*>(gameObject->AddComponent<dae::BurgerHolder>());
			burgerHolder->Init(TrimJson(compIt.value()["type"]),colliders);
		}
		break;
		default:
			std::cout << "NO KNOWN COMPONENT TYPE \n";
			break;
		}
	}

	//children
	for (auto childIt = it["children"].begin(); childIt != it["children"].end(); ++childIt)
	{
		std::ifstream prefabFile("..\\Data\\Prefabs\\" + TrimJson(childIt.value()["name"]), std::ifstream::binary);
		json j = json::parse(prefabFile);
		prefabFile.close();
		j["x"] = childIt.value()["x"];
		j["y"] = childIt.value()["y"];
		j["z"] = childIt.value()["z"];
		ConstructGO(j, colliders)->SetParent(gameObject);
	}

	return gameObject;
}