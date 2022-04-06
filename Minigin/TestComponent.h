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
		void OnInputX(float);
		float m_Test;
	};
	
}


