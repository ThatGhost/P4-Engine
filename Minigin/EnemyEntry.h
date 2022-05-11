#pragma once
#include "Component.h"
#include "EnemyComponent.h"

namespace dae
{
	class EnemyEntry : public Component
	{
	public:
		EnemyEntry(GameObject*);
		~EnemyEntry() override;

		virtual void Update(float) override;
		void Init(float dest, dae::EnemyType type) { m_dest = dest; m_original = GetOwner()->GetPosition().GetPosition().x; m_type = type; }
	private:
		float m_dest{};
		float m_original{};
		const float m_Speed{50};
		EnemyType m_type{EnemyType::sausage};
	};
}

