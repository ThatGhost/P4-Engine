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
	private:

	};
}

