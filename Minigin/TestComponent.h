#pragma once
#include "Component.h"

namespace dae
{
	class TestComponent : public Component
	{
	public:
		TestComponent(GameObject* Owner);
		virtual ~TestComponent() override;

	private:
		void Test(float);
		float m_Test;
	};
	
}


