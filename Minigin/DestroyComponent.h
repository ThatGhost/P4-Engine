#pragma once
#include "Component.h"

namespace dae {
	class DestroyComponent : public Component
	{
	public:
		DestroyComponent(GameObject*);
		~DestroyComponent() override {}

		void Init(float sec) { m_Time = sec; m_Run = true; }
		virtual void Update(float) override;
	private:
		float m_Timer{};
		float m_Time{};
		bool m_Run{false};
	};
}
