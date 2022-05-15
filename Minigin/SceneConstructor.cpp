#include "MiniginPCH.h"
#include "SceneConstructor.h"
#include <fstream>
#include <map>
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"

#include "Collider.h"
#include "TestComponent.h"
#include "RenderComponent.h"
#include "PlayerController.h"
#include "BurgerComponent.h"
#include "MainMenu.h"
#include "EnemySpawner.h"
#include "GameManager.h"
#include "GameManagerVersus.h"
#include "GameManagerSolo.h"
#include "GameManagerCoop.h"
#include "GameManagerOnline.h"
#include "EnemyPlayerComponent.h"
#include "ServerMenu.h"

using json = nlohmann::json;
bool m_canRecreate = false;

std::string dae::SceneConstructor::TrimJson(const std::string& string)
{
	return string.substr(0, string.size());
}

void dae::SceneConstructor::Init()
{
}

void dae::SceneConstructor::ConstructScene(const std::string& nameScene, bool kill)
{
	if(kill)
		SceneManager::GetInstance().KillMainScene();

	//read file
	std::ifstream SceneFile("..\\Data\\Levels\\"+nameScene, std::ifstream::binary);
	json sceneData;
	sceneData = json::parse(SceneFile);
	SceneFile.close();

	//make scene
	auto scene = SceneManager::GetInstance().CreateScene(TrimJson(sceneData["name"]));
	std::vector<dae::Collider*>* colliders = scene->GetColliderVector();

	//construct the gameobjects
	for (auto it = sceneData["gameObjects"].begin(); it != sceneData["gameObjects"].end(); ++it)
	{
		json j{ it.value()};
		ConstructGO(j, colliders, scene);
	}

	//construct the prefabs
	for (auto it = sceneData["prefabs"].begin(); it != sceneData["prefabs"].end(); ++it)
	{
		std::ifstream prefabFile("..\\Data\\Prefabs\\" + TrimJson(it.value()["name"]), std::ifstream::binary);
		json j = json::parse(prefabFile);
		prefabFile.close();
		j["x"] = it.value()["x"];
		j["y"] = it.value()["y"];
		j["z"] = it.value()["z"];
		ConstructGO(j,colliders,scene);
	}

	//test comp
	//GameObject* go = new GameObject();
	//go->AddComponent<TestComponent>();
	//scene.Add(go);

	m_canRecreate = true;
}

dae::GameObject* dae::SceneConstructor::ConstructGO(const json& it, std::vector<dae::Collider*>* colliders, Scene* scene)
{
	dae::GameObject* gameObject = scene->Add(new GameObject());

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
		AddComponent(compIt,gameObject,colliders);
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
		ConstructGO(j, colliders,scene)->SetParent(gameObject);
	}

	return gameObject;
}

void dae::SceneConstructor::AddComponent(const json::const_iterator& compIt, GameObject* gameObject, std::vector<dae::Collider*>* colliders)
{
	std::string NameComponent{ TrimJson(compIt.value()["name"]) };
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

		//tag of collider
		std::string tag{ TrimJson(compIt.value()["tag"]) };
		if (tag != "NULL")
			coll->SetTag(tag);

		//to look or be looked at
		bool seen{ TrimJson(compIt.value()["seen"]) == "true"};
		bool look{ TrimJson(compIt.value()["look"]) == "true"};
		coll->Setlookers(seen,look);

		//put collider in the big list
		colliders->push_back(coll);
	}
	break;
	case Components::renderComponent:
	{
		dae::RenderComponent* render = static_cast<dae::RenderComponent*>(gameObject->AddComponent<dae::RenderComponent>());
		bool animated = TrimJson(compIt.value()["animated"]) == "true";
		float scale = std::stof(TrimJson(compIt.value()["scale"]));

		render->SetImage(TrimJson(compIt.value()["image"]), animated,scale);

		if (animated)
		{
			int frames = std::stoi(TrimJson(compIt.value()["frames"]));
			float frameTime = std::stof(TrimJson(compIt.value()["frameTime"]));
			render->SetAnimationDetails(frames,frameTime);
		}
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
	case Components::burgerComponent:
	{
		BurgerComponent* burger = static_cast<BurgerComponent*>(gameObject->AddComponent<dae::BurgerComponent>());
		burger->Init(TrimJson(compIt.value()["bun"]) == "true");
	}
	break;
	case Components::MainMenu:
	{
		gameObject->AddComponent<dae::MainMenu>();
	}
	break;
	case Components::EnemySpawner:
	{
		gameObject->AddComponent<dae::EnemySpawner>();
	}
	break;
	case Components::GameManager:
	{
		std::string type{TrimJson(compIt.value()["type"])};
		GameManager* gamemanager = nullptr;

		if (type == "solo")
		{
			gamemanager = static_cast<GameManagerSolo*>(gameObject->AddComponent<GameManagerSolo>());
			static_cast<GameManagerSolo*>(gamemanager)->SetLevel(std::stoi(TrimJson(compIt.value()["level"])));
		}
		else if (type == "versus")
		{
			gamemanager = static_cast<GameManagerVersus*>(gameObject->AddComponent<GameManagerVersus>());
		}
		else if (type == "coop")
		{
			gamemanager = static_cast<GameManagerCoop*>(gameObject->AddComponent<GameManagerCoop>());
		}
		else if (type == "online")
		{
			gamemanager = static_cast<GameManagerOnline*>(gameObject->AddComponent<GameManagerOnline>());
		}
		SceneManager::GetInstance().SetGameManager(gamemanager);
	}
	break;
	case Components::EnemyPlayerComponent:
	{
		gameObject->AddComponent<EnemyPlayerComponent>();
	}
	break;
	case Components::ServerMenu:
	{
		gameObject->AddComponent<ServerMenu>();
	}
	break;
	default:
		std::cout << "NO KNOWN COMPONENT TYPE \n";
		break;
	}
}