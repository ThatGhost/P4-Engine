#pragma once
#include "Component.h"

namespace dae
{
	class ServerMenu :	public Component
	{
	public:
		ServerMenu(GameObject*);
		~ServerMenu() override;

		virtual void Update(float) override;


		void SetSwitchScene(bool b) { m_switchScenes = b; }
	private:
		bool m_switchScenes{false};
	};
}

