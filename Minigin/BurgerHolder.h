#pragma once
#include "Component.h"

namespace dae
{
	class BurgerComponent;
	class Collider;
	class BurgerHolder : public Component
	{
	public:
		BurgerHolder(GameObject* Owner);
		virtual ~BurgerHolder() override;

		virtual void Update(float) override;
		void Init(const std::string& type, std::vector<dae::Collider*>* colliders);
		void OnCollideNewBurger(float);
	private:
		BurgerComponent* m_burgerPart;
		float m_Argument{};
		bool m_NewBurger{false};
	};
}

