#pragma once
#include "Component.h"
namespace dae
{
	class EnemyEntry : public Component
	{
	public:
		EnemyEntry(GameObject*);
		~EnemyEntry() override;

		virtual void Update(float) override;
		void Init(float dest) { m_dest = dest; m_original = GetOwner()->GetPosition().GetPosition().x; }
	private:
		float m_dest;
		float m_original;
		const float m_Speed{50};
	};
}

