#pragma once
#include "Component.h"
namespace dae
{
	class BurgerComponent : public Component
	{
	public:
		BurgerComponent(GameObject* Owner);
		virtual ~BurgerComponent() override;

		virtual void Update(float deltaTime) override;

		void SetIsFalling(bool isFalling) { m_isFalling = isFalling; }
		int GetId() { return m_BurgerId; }
	private:
		const float m_FallSpeed{20};
		bool m_isFalling{false};
		int m_BurgerId;
	};
}

