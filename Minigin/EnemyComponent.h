#pragma once
#include "Component.h"

namespace dae
{
	class EnemySpawner;
	class Collider;
	using byte = unsigned char;

	class EnemyComponent : public Component
	{
	public:
		EnemyComponent(GameObject*);
		~EnemyComponent() override;

		virtual void Update(float) override;
		virtual void OnCollisionEnter(Collider*, Collider*) override;
	private:
		EnemySpawner* m_spawner{ nullptr };
		RenderComponent* m_renderer{ nullptr };

		void Die();
		void Salt();

		void OnY(byte*);
		byte* m_Argument{ nullptr };

		enum class state{
			init,
			normal,
			salted,
		};
		state m_state{state::init};
		bool m_isDead{false};

	};
}

