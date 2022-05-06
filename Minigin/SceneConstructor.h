#pragma once
#include <map>
#include "json.hpp"
#include "EventManager.h"
#include "InputManager.h"
#include "json.hpp"

namespace dae
{
	using json = nlohmann::json;
	enum class Components
	{
		testComponent,
		collider,
		renderComponent,
		playerController,
		burgerComponent,
		MainMenu,
	};

	const std::map<std::string, Components> StringToType{
		{"Collider", Components::collider},
		{"RenderComponent", Components::renderComponent},
		{"TestComponent", Components::testComponent},
		{"PlayerController", Components::playerController},
		{"BurgerPart", Components::burgerComponent},
		{"MainMenu", Components::MainMenu},
	};

	class Scene;
	class Collider;
	class GameObject;

	class SceneConstructor
	{
	public:
		static void OnReCreateScene(float);
		static void Init();

		static void ConstructScene(const std::string&);
	private:
		static std::string TrimJson(const std::string& string);
		static dae::GameObject* ConstructGO(const nlohmann::json& it, std::vector<dae::Collider*>* colliders);
		static void AddComponent(const json::const_iterator&, GameObject*, std::vector<dae::Collider*>*);
	};
}

