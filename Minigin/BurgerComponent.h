#pragma once
#include "Component.h"
namespace dae
{
	using byte = unsigned char;
	class BurgerComponent : public Component
	{
	public:
		BurgerComponent(GameObject* Owner);
		virtual ~BurgerComponent() override;


		virtual void Update(float deltaTime) override;
		virtual void OnCollisionEnter(Collider*, Collider*) override;
		virtual void OnCollision(Collider*, Collider*) override;

		void SetIsFalling(bool isFalling) { m_IsFalling = isFalling; }
		void Init(bool bread = true);
		bool IsDone() { return m_Done; }
		bool IsFalling() { return m_IsFalling; }
	private:
		void AddScore(int);

		const float m_FallSpeed{8};
		const float m_MaxSpeed{40};

		bool m_WalkSpots[5]{false};

		float m_FallVelocity{};
		bool m_IsFalling{true};
		bool m_Done{ false };
		bool m_Init{true};
	};
}

