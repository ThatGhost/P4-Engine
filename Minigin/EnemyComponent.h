#pragma once
#include "Component.h"

namespace dae
{
	class EnemySpawner;
	class Collider;
	class PlayerController;
	using byte = unsigned char;

	enum class EnemyType
	{
		sausage,
		egg,
		pickle
	};

	class EnemyComponent : public Component
	{
	public:
		EnemyComponent(GameObject*);
		~EnemyComponent() override;

		void Init(EnemyType);
		virtual void Update(float) override;
		virtual void OnCollisionEnter(Collider*, Collider*) override;
		virtual void OnCollision(Collider*, Collider*) override;
	private:
		EnemySpawner* m_Spawner{ nullptr };
		RenderComponent* m_Renderer{ nullptr };
		GameObject* m_Player{nullptr};

		const float m_Speed{40};
		const float m_PepperTime{5};

		void Die();
		void Salt();
		void DecideDirection();
		void SnapToStair();
		void SnapToPlatform();

		enum class EnemyMoveState
		{
			Up = 0,
			Down = 1,
			Left = 2,
			Right = 3,
		};
		EnemyMoveState m_moveState{ EnemyMoveState::Right };
		EnemyType m_type{EnemyType::sausage};
		bool m_CanUp{ false };
		bool m_CanDown{ false };
		bool m_CanLeft{ false };
		bool m_CanRight{ false };
		bool m_OnPlatform{ true };
		bool m_Reset{ false };
		bool m_Peppered{ false };
		bool m_RandomWalk{ false };

		float m_PepperTimer{};
		float m_StairPosition{};
		float m_PlatformPosition{};

		int m_Score{};
	};
}

