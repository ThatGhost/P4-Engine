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
		void Init(float dest, dae::EnemyType type) { m_Dest = dest; m_Original = GetOwner()->GetPosition().GetPosition().x; m_Type = type; }
	private:
		float m_Dest{};
		float m_Original{};
		const float m_Speed{50};
		EnemyType m_Type{EnemyType::sausage};
	};
}

