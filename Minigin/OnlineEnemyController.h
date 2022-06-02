#pragma once
#include "Component.h"
namespace dae
{
	class RenderComponent;
	class OnlineEnemyController : public Component
	{
	public:
		OnlineEnemyController(GameObject* owner);
		~OnlineEnemyController() override{}

		void UpdatePlayer(float x, float y, int state)
		{
			glm::vec3 position{x,y,0};
			GetOwner()->SetPosition(position);
			state;
		}

		void Kill()
		{

		}

		void Salt()
		{

		}

		void SetGood(bool good);

	private:
		bool m_Good{};
		RenderComponent* m_renderer;
	};
}

