#if _WIN64
#define ENVIROMENT64
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

		void SetGood(bool good);
		void SetState(int state);
		void ShootPepper();

	private:
		int m_State{0};
		bool m_Good{true};
		RenderComponent* m_Renderer;
	};
}
#endif
