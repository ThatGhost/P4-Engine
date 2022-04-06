#pragma once
#include <map>
#include "json.hpp"

namespace dae
{
	enum class Components
	{
		testComponent,
		collider,
		renderComponent
	};

	const std::map<std::string, Components> StringToType{
		{"Collider", Components::collider},
		{"RenderComponent", Components::renderComponent},
		{"TestComponent", Components::testComponent},
	};

	class Scene;
	class Collider;
	class GameObject;

	class SceneConstructor
	{
	public:
		static void ConstructScene(const std::string&);
	private:
		static std::string TrimJson(const std::string& string);
		static dae::GameObject* ConstructGO(const nlohmann::json::iterator& it, std::vector<dae::Collider*>* colliders);

	};
}

